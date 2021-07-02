CFLAGS = -lm
all: heat

heat : heat.c
	   h5pcc heat.c ./library/mean.c ./library/mean.h ./library/derivative.c ./library/derivative.h -o heat $(CFLAGS)
clean :
	rm *.exe *.o  *.h5 *.gch heat

