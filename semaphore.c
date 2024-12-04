#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>
#include <assert.h>

void critical(const char *s) {
    int length = strlen(s);
    for (int i = 0; i < length; i++) {
        printf("%c", s[i]);
    }
    printf("\n");
}

typedef volatile struct {
    volatile atomic_int value;
    volatile atomic_flag mutex;
} Semaphore;
Semaphore semaphore;

#define acquire(m) while (atomic_flag_test_and_set(m))
#define release(m) atomic_flag_clear(m)

int wait(Semaphore *s) {
    acquire(&s->mutex);
    while (atomic_load(&s->value) <= 0);
    atomic_fetch_sub(&s->value, 1);
    release(&s->mutex);
    return 0;
}

int signal(Semaphore *s) {
    return atomic_fetch_add(&s->value, 1);
}

void *f(void *p) {
    char *message = (char *)p;
    for (;;) {
        wait(&semaphore);
        critical(message);
        signal(&semaphore);
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    assert(ATOMIC_INT_LOCK_FREE == 2);
    atomic_init(&semaphore.value, 1);
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, f, "test1");
    pthread_create(&threads[2], NULL, f, "test2");
    for (;;);
    return 0;
}
