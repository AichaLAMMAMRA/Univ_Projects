CC=gcc
CFLAGS=-I. -lm -O0
DEPS = fonctionSeq.h
OBJ = main.o fonctionSeq.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

project: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)