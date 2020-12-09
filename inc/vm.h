#ifndef __VM_H
#define __VM_H

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
    Obj_closure* closure;
    uint8_t* ip;
    Value* slots;
} Call_frame;

typedef struct {
    Call_frame frames[FRAMES_MAX];
    int frame_count;

    Value stack[STACK_MAX];
    Value* stack_top;
    Table strings;
    Table globals;

    Obj_string* init_string;
    Obj_upvalue* open_upvalues;

    size_t bytes_allocated;
    size_t next_GC;

    Obj* objects;
    int gray_count;
    int gray_capacity;
    Obj** gray_stack;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} Interpret_result;

extern VM vm;

void init_VM();
void free_VM();
Interpret_result interpret(const char* source);
void push(Value value);
Value pop();

#endif // __VM_H
