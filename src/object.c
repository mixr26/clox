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
    object->is_marked = false;

    object->next = vm.objects;
    vm.objects = object;

#ifdef DEBUG_LOG_GC
    printf("%p allocate %ld for %d\n", (void*)object, size, type);
#endif

    return object;
}

Obj_bound_method* new_bound_method(Value receiver, Obj_closure *method) {
    Obj_bound_method* bound = ALLOCATE_OBJ(Obj_bound_method, OBJ_BOUND_METHOD);
    bound->receiver = receiver;
    bound->method = method;
    return bound;
}

Obj_class* new_class(Obj_string *name) {
    Obj_class* klass = ALLOCATE_OBJ(Obj_class, OBJ_CLASS);
    klass->name = name;
    init_table(&klass->methods);
    return klass;
}

Obj_closure* new_closure(Obj_function *function) {
    Obj_upvalue** upvalues = ALLOCATE(Obj_upvalue*, function->upvalue_count);
    for (int i = 0; i < function->upvalue_count; i++)
        upvalues[i] = NULL;

    Obj_closure* closure = ALLOCATE_OBJ(Obj_closure, OBJ_CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalue_count = function->upvalue_count;
    return closure;
}

Obj_function* new_function() {
    Obj_function* function = ALLOCATE_OBJ(Obj_function, OBJ_FUNCTION);

    function->arity = 0;
    function->upvalue_count = 0;
    function->name = NULL;
    init_chunk(&function->chunk);
    return function;
}

Obj_instance* new_instance(Obj_class *klass) {
    Obj_instance* instance = ALLOCATE_OBJ(Obj_instance, OBJ_INSTANCE);
    instance->klass = klass;
    init_table(&instance->fields);
    return instance;
}

Obj_native* new_native(Native_fn function) {
    Obj_native* native = ALLOCATE_OBJ(Obj_native, OBJ_NATIVE);
    native->function = function;
    return native;
}

static Obj_string* allocate_string(char* chars, int length,
                                   uint32_t hash) {
    Obj_string* string = ALLOCATE_OBJ(Obj_string, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    push(OBJ_VAL(string));
    table_set(&vm.strings, string, NIL_VAL);
    pop();

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

Obj_upvalue* new_upvalue(Value *slot) {
    Obj_upvalue* upvalue = ALLOCATE_OBJ(Obj_upvalue, OBJ_UPVALUE);
    upvalue->closed = NIL_VAL;
    upvalue->location = slot;
    upvalue->next = NULL;
    return upvalue;
}

static void print_function(Obj_function* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->chars);
}

void print_object(Value value) {
    switch (OBJ_TYPE(value)) {
    case OBJ_BOUND_METHOD:
        print_function(AS_BOUND_METHOD(value)->method->function);
        break;
    case OBJ_CLASS:
        printf("%s", AS_CLASS(value)->name->chars);
        break;
    case OBJ_CLOSURE:
        print_function(AS_CLOSURE(value)->function);
        break;
    case OBJ_FUNCTION:
        print_function(AS_FUNCTION(value));
        break;
    case OBJ_INSTANCE:
        printf("%s instance", AS_INSTANCE(value)->klass->name->chars);
        break;
    case OBJ_NATIVE:
        printf("<native fn>");
        break;
    case OBJ_STRING:
        printf("%s", AS_CSTRING(value));
        break;
    case OBJ_UPVALUE:
        printf("upvalue");
        break;
    }
}
