#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, object_type) \
    (type*)allocate_object(sizeof(type), object_type)

static Obj* allocate_object(size_t size, Obj_type type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;

    object->next = vm.objects;
    vm.objects = object;
    return object;
}

static Obj_string* allocate_string(char* chars, int length,
                                   uint32_t hash) {
    Obj_string* string = ALLOCATE_OBJ(Obj_string, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    table_set(&vm.strings, string, NIL_VAL);

    return string;
}

static uint32_t hash_string(const char* key, int length) {
    uint32_t hash = 2166136261u;

    for (int i = 0; i < length; i++) {
        hash ^= key[i];
        hash *= 16777619;
    }

    return hash;
}

Obj_string* take_string(char *chars, int length) {
    uint32_t hash = hash_string(chars, length);
    Obj_string* interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocate_string(chars, length, hash);
}

Obj_string* copy_string(const char *chars, int length) {
    uint32_t hash = hash_string(chars, length);
    Obj_string* interned = table_find_string(&vm.strings, chars, length, hash);
    if (interned)
        return interned;

    char* heap_chars = ALLOCATE(char, length + 1);
    memcpy(heap_chars, chars, length);
    heap_chars[length] = '\0';

    return allocate_string(heap_chars, length, hash);
}

void print_object(Value value) {
    switch (OBJ_TYPE(value)) {
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    }
}
