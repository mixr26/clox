#ifndef __OBJECT_H
#define __OBJECT_H

#include "common.h"
#include "chunk.h"
#include "value.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_FUNCTION(value)      is_obj_type(value, OBJ_FUNCTION)
#define IS_NATIVE(value)        is_obj_type(value, OBJ_NATIVE)
#define IS_STRING(value)        is_obj_type(value, OBJ_STRING)

#define AS_FUNCTION(value)      ((Obj_function*)AS_OBJ(value))
#define AS_NATIVE(value) \
    (((Obj_native*)AS_OBJ(value))->function)
#define AS_STRING(value)        ((Obj_string*)AS_OBJ(value))
#define AS_CSTRING(value)       (((Obj_string*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_FUNCTION,
    OBJ_NATIVE,
    OBJ_STRING
} Obj_type;

struct Obj {
    Obj_type type;
    struct Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    Obj_string* name;
} Obj_function;

typedef Value (*Native_fn)(int arg_count, Value* args);

typedef struct {
    Obj obj;
    Native_fn function;
} Obj_native;

struct Obj_string {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

Obj_function* new_function();
Obj_native* new_native(Native_fn function);
Obj_string* take_string(char* chars, int length);
Obj_string* copy_string(const char* chars, int length);
void print_object(Value value);

static inline bool is_obj_type(Value value, Obj_type type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif // __OBJECT_H
