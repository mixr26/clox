TARGET	= clox 

Q ?= @
PREFIX ?= 

INC_DIR = inc
OUT_DIR = out
BIN_DIR = bin
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

SRC 	= $(wildcard $(SRC_DIR)/*.c)
OBJ 	= $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC:.c=.o))

CXX = ${PREFIX}g++
CC  = ${PREFIX}gcc
LD  = ${PREFIX}ld
AS  = ${PREFIX}gcc -x assembler-with-cpp
CP  = ${PREFIX}objcopy
OD  = ${PREFIX}objdump
SZ  = ${PREFIX}size

CFLAGS = -Wall -std=c99 -O0 -g
CFLAGS += -I./$(INC_DIR)

LFLAGS  = -L./$(OUT_DIR)/$(LIB_DIR)

all : mkobjdir $(TARGET)

$(TARGET) : $(OBJ)
	@echo "  [LD]      $@"
	$(Q)$(CC) -o $(OUT_DIR)/$(BIN_DIR)/$@ $^ $(LFLAGS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo "  [CC]     $<"
	$(Q)$(CC) $(CFLAGS) -c  $< -o $@

help :
	@echo "  [SRC]:      $(SRC)"
	@echo
	@echo "  [OBJ]:     $(OBJ)"

clean :
	@echo "  [RM]    $(OBJ)"
	@$(RM) $(OBJ)
	@echo
	@echo "  [RM]     $(TARGET) "
	@$(RM) $(OUT_DIR)/$(BIN_DIR)/$(TARGET)

mkobjdir :
	@mkdir -p obj

.PHONY : all run deploy help clean formatsource mkobjdir
