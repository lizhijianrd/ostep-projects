fLAGS = -g -Wall -Werror

MACROS = # -DDEBUG

all: wish

wish: wish.c error.h error.o path.h path.o builtin.h builtin.o utils.h utils.o
	gcc $(MACROS) $(FLAGS) -o wish wish.c builtin.o error.o path.o utils.o

builtin.o: builtin.c error.h
	gcc $(MACROS) $(FLAGS) -c builtin.c

path.o: path.c error.h
	gcc $(MACROS) $(FLAGS) -c path.c

utils.o: utils.c
	gcc $(MACROS) $(FLAGS) -c utils.c

error.o: error.c
	gcc $(MACROS) $(FLAGS) -c error.c

clean:
	rm -f wish builtin.o utils.o error.o path.o

