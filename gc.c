#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_MAX 256
#define INITIAL_MAX_OBJECTS 8

#ifndef NDEBUG
#define assert(condition) \
    if (!(condition)) { \
        printf("file: %s, line: %d assertion %s failed \n", \
            __FILE__, __LINE__, "'"#condition"'"); \
        exit(1); \
    }
#else
#define assert(condition)
#endif


typedef enum {
    OBJ_INT,
    OBJ_PAIR,
} ObjectType;

typedef struct SObject {
    ObjectType type;
    bool marked;

    struct SObject *next;

    union {
        int value;

        struct {
            struct SObject *head;
            struct SObject *tail;
        };
    };
} Object;


typedef struct {
    Object *stack[STACK_MAX];
    int stack_size;

    Object *first;

    int nobjects;
    int max_objects;

} VM;

VM *new_vm() {
    VM *vm = malloc(sizeof(VM));
    vm->stack_size = 0;
    vm->first = NULL;
    vm->nobjects = 0;
    vm->max_objects = INITIAL_MAX_OBJECTS;
    return vm;
}

void push(VM *vm, Object *value) {
    assert(vm->stack_size < STACK_MAX);
    vm->stack[vm->stack_size++] = value;
}

Object *pop(VM *vm) {
    assert(vm->stack_size > 0);
    return vm->stack[--vm->stack_size];
}

void mark(Object *object) {
    if (object->marked) return;

    object->marked = true;

    if (object->type == OBJ_PAIR) {
        mark(object->head);
        mark(object->tail);
    }
}

void mark_all(VM *vm) {
    for (int i = 0; i < vm->stack_size; i++) {
        mark(vm->stack[i]);
    }
}

void sweep(VM *vm) {
    Object **object = &vm->first;

    while (*object) {
        if (!(*object)->marked) {
            Object *unreached = *object;
            *object = unreached->next;

            free(unreached);
            vm->nobjects--;
        } else {
            (*object)->marked = false;
            object = &(*object)->next;
        }
    }
}

void gc(VM *vm) {
    int nobjects = vm->nobjects;
    mark_all(vm);
    sweep(vm);

    if (vm->nobjects == 0) {
        vm->max_objects = INITIAL_MAX_OBJECTS;
    } else {
        vm->max_objects = 2 * vm->nobjects;
    }

    printf("collected %d objects, %d remaining\n", (nobjects - vm->nobjects),
        vm->nobjects);
}

Object *new_object(VM *vm, ObjectType type) {
    if (vm->nobjects == vm->max_objects) gc(vm);

    Object *object = malloc(sizeof(Object));
    object->type = type;
    object->marked = false;

    object->next = vm->first;
    vm->first = object;
    vm->nobjects++;

    return object;
}

void push_int(VM *vm, int int_value) {
    Object *object = new_object(vm, OBJ_INT);
    object->value = int_value;

    push(vm, object);
}

Object *push_pair(VM *vm) {
    Object *object = new_object(vm, OBJ_PAIR);
    object->tail = pop(vm);
    object->head = pop(vm);

    push(vm, object);
    return object;
}

void print_object(Object *object) {
    switch (object->type) {
        case OBJ_INT:
            printf("%d", object->value);

        case OBJ_PAIR:
            printf("(");
            print_object(object->head);
            printf(", ");
            print_object(object->tail);
            printf(")");
            break;
    }
}

void free_vm(VM *vm) {
    vm->stack_size = 0;
    gc(vm);
    free(vm);
}

void test_assert() {
    assert(false);
}

void test1() {
    printf("Test 1: Objects on the stack are preserved.\n");

    VM *vm = new_vm();

    push_int(vm, 1);
    push_int(vm, 2);

    gc(vm);
    assert(vm->nobjects == 2);
    free_vm(vm);
}

void test2() {
    printf("Test 2: Unreached objects are collected.\n");
    VM *vm = new_vm();

    push_int(vm, 1);
    push_int(vm, 2);
    pop(vm);
    pop(vm);

    gc(vm);
    assert(vm->nobjects == 0);
    free_vm(vm);
}

void test3() {
    printf("Test 3: Reach nested objects\n");
    VM *vm = new_vm();

    push_int(vm, 1);
    push_int(vm, 2);
    push_pair(vm);
    push_int(vm, 3);
    push_int(vm, 4);
    push_pair(vm);
    push_pair(vm);

    gc(vm);
    assert(vm->nobjects == 7);
    free_vm(vm);
}

void test4() {
    printf("Test 4: Handle cycles\n");
    VM *vm = new_vm();
    push_int(vm, 1);
    push_int(vm, 2);

    Object *object1 = push_pair(vm);
    push_int(vm, 3);
    push_int(vm, 4);
    Object *object2 = push_pair(vm);

    object1->tail = object2;
    object2->tail = object1;

    gc(vm);
    assert(vm->nobjects == 4);
    free_vm(vm);
}

void perf_test() {
    printf("Performance Test.\n");

    VM *vm = new_vm();

    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 20; j++) {
            push_int(vm, i);
        }

        for (int k = 0; k < 20; k++) {
            pop(vm);
        }
    }
    free_vm(vm);
}

int main(int argc, char *argv[]) {
    // test_assert();
    test1();
    test2();
    test3();
    test4();
    perf_test();

    return 0;
}
