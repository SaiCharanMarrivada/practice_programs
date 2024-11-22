#include <stddef.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

typedef union Header {
    struct {
        union Header *next;
        size_t size;
        bool is_free;
    };
    // to make sure that header is always 16 byte aligned
    char alignment[16];
} Header;

Header *head = NULL;
Header *tail = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

Header *get_free_block(size_t size) {
    Header *current = head;
    while (current) {
        if (current->is_free && current->size) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void free(void *p) {
    Header *header, *temp;

    if (!p) return;
    pthread_mutex_lock(&lock);
    void *program_break = sbrk(0);
    header = (Header *)p - 1;

    if ((char *)p + header->size == program_break) {
        if (head == tail) {
            head = NULL;
            tail = NULL;
        } else {
            temp = head;
            while (temp) {
                if (temp->next == tail) {
                    temp->next = NULL;
                    tail = temp;
                    break;
                }
                temp = temp->next;
            }
        }
        sbrk(0 - header->size - sizeof(Header));
        pthread_mutex_unlock(&lock);
        return;
    }

    header->is_free = true;
    pthread_mutex_unlock(&lock);
}


void *malloc(size_t size) {
    void *block;
    Header *header;

    pthread_mutex_lock(&lock);
    header = get_free_block(size);

    if (header) {
        header->is_free = false;
        pthread_mutex_unlock(&lock);
        return (void *)(header + 1);
    }

    int totalsize = size + sizeof(Header);
    block = sbrk(totalsize);
    if (block == (void *)-1) {
        pthread_mutex_unlock(&lock);
        return NULL;
    }
    header = block;
    header->size = totalsize;
    header->is_free = false;
    header->next = NULL;

    if (!head) head = header;

    if (tail) tail->next = header;
    pthread_mutex_unlock(&lock);
    return (void *)(header + 1);
}

void *calloc(size_t n, size_t size) {
    if (n == 0 || size == 0) {
        return NULL;
    }
    void *block = malloc(n * size);
    if (!block) {
        return NULL;
    }
    memset(block, 0, size);
    return block;
}

void *realloc(void *block, size_t size) {
    if (size == 0 || block == NULL) return NULL;
    Header *header = (Header *)block - 1;

    if (header->size >= size) {
        return block;
    }

    void *newblock = malloc(size);

    if (newblock != NULL) {
        memcpy(newblock, block, header->size);
        free(block);
    }
    return block;
}
