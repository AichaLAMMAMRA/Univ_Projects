CC=gcc
CFLAGS=-fopenmp -I. -lm -O0
DEPS = fonctionParll.h
OBJ = main.o fonctionParll.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

project: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)