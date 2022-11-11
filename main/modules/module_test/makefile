CC=gcc
INCLUDE=
CFLAG=-g

# SRC=script_tst.c ../script_engine/parser.c
# SRC=crc_tst.c
SRC=interactive_tst.c
OBJ=$(subst ".c",".o",$(SRC))
# TARGET=script_tst
TARGET=interactive_tst

$(TARGET):$(OBJ)
	$(CC) $(CFLAG) $^ -o $@


%.o:%.c
	$(CC) $(CFLAG) $^ -o $@

.PHONY: debug
debug:
	@echo $(OBJ)