#ifndef __COMPILER_H
#define __COMPILER_H

#include "object.h"
#include "vm.h"

bool compile(const char* source, Chunk* chunk);

#endif
