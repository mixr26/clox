#ifndef __COMPILER_H
#define __COMPILER_H

#include "object.h"
#include "vm.h"

Obj_function* compile(const char* source);
void mark_compiler_roots();

#endif
