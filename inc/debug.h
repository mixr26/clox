#ifndef __DEBUG_H
#define __DEBUG_H

#include "chunk.h"

void disassemble_chunk(Chunk* chunk, const char* name);
int disassemble_instruction(Chunk* chunk, int offset);

#endif // __DEBUG_H
