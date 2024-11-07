CFLAGS = -Wall -Wextra -pedantic -std=c99

all: nihonC.exe

nihonC.exe: nihonC.c
	gcc nihonC.c $(CFLAGS) -o nihonC.exe

compile: nihonC.exe
	./nihonC.exe $(ARGS)

clean:
	del nihonC.exe