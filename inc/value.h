#ifndef __VALUE_H
#define __VALUE_H

#include "common.h"

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} Value_array;

void init_value_array(Value_array* array);
void write_value_array(Value_array* array, Value value);
void free_value_array(Value_array* array);
void print_value(Value value);

#endif // __VALUE_H
