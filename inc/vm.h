#ifndef __VM_H
#define __VM_H

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stack_top;
    Table strings;
    Table globals;

    Obj* objects;
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
