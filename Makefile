CC          =	gcc
# Flags that are sent to the compiler  change the std if you wish

CFLAGS      =	-Wall -std=c99 -pedantic -g

prog: src/main.c src/Htable.c
	$(CC) $(CFLAGS) src/main.c src/Htable.c  -Iinclude -o bin/run

test:
	$(CC) $(CFLAGS) src/testMain.c src/Htable.c src/test.c -Iinclude -o bin/test

cppcheck:
	cppcheck --enable=all --language=c --std=c99 --inconclusive --suppress=missingInclude src/*.c -i ./include

clean:
	rm bin/*
