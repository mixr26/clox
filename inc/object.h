#ifndef __OBJECT_H
#define __OBJECT_H

#include "common.h"
#include "chunk.h"
#include "table.h"
#include "value.h"

#define OBJ_TYPE(value)         (AS_OBJ(value)->type)

#define IS_CLASS(value)         is_obj_type(value, OBJ_CLASS)
#define IS_CLOSURE(value)       is_obj_type(value, OBJ_CLOSURE)
#define IS_FUNCTION(value)      is_obj_type(value, OBJ_FUNCTION)
#define IS_INSTANCE(value)      is_obj_type(value, OBJ_INSTANCE)
#define IS_NATIVE(value)        is_obj_type(value, OBJ_NATIVE)
#define IS_STRING(value)        is_obj_type(value, OBJ_STRING)

#define AS_CLASS(value)         ((Obj_class*)AS_OBJ(value))
#define AS_CLOSURE(value)       ((Obj_closure*)AS_OBJ(value))
#define AS_FUNCTION(value)      ((Obj_function*)AS_OBJ(value))
#define AS_INSTANCE(value)      ((Obj_instance*)AS_OBJ(value))
#define AS_NATIVE(value) \
    (((Obj_native*)AS_OBJ(value))->function)
#define AS_STRING(value)        ((Obj_string*)AS_OBJ(value))
#define AS_CSTRING(value)       (((Obj_string*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_CLASS,
    OBJ_CLOSURE,
    OBJ_FUNCTION,
    OBJ_INSTANCE,
    OBJ_NATIVE,
    OBJ_STRING,
    OBJ_UPVALUE
} Obj_type;

struct Obj {
    Obj_type type;
    bool is_marked;
    struct Obj* next;
};

typedef struct {
    Obj obj;
    int arity;
    int upvalue_count;
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

typedef struct Obj_upvalue {
    Obj obj;
    Value* location;
    Value closed;
    struct Obj_upvalue* next;
} Obj_upvalue;

typedef struct {
    Obj obj;
    Obj_function* function;
    Obj_upvalue** upvalues;
    int upvalue_count;
} Obj_closure;

typedef struct {
    Obj obj;
    Obj_string* name;
} Obj_class;

typedef struct {
    Obj obj;
    Obj_class* klass;
    Table fields;
} Obj_instance;

Obj_class* new_class(Obj_string* name);
Obj_closure* new_closure(Obj_function* function);
Obj_function* new_function();
Obj_instance* new_instance(Obj_class* klass);
Obj_native* new_native(Native_fn function);
Obj_string* take_string(char* chars, int length);
Obj_string* copy_string(const char* chars, int length);
Obj_upvalue* new_upvalue(Value* slot);
void print_object(Value value);

static inline bool is_obj_type(Value value, Obj_type type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif // __OBJECT_H
