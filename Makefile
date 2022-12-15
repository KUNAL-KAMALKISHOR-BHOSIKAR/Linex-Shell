out: main.o jobs.o process_functions.o discover.o process.o builtin.o input.o parse.o display.o username.o
	gcc main.o jobs.o process_functions.o discover.o process.o builtin.o input.o parse.o display.o username.o -o out

main.o: main.c
	gcc -c main.c

jobs.o: jobs.c
	gcc -c jobs.c

process_functions.o: process_functions.c
	gcc -c process_functions.c

discover.o: discover.c
	gcc -c discover.c

process.o: process.c
	gcc -c process.c

builtin.o: builtin.c
	gcc -c builtin.c

input.o: input.c
	gcc -c input.c

parse.o: parse.c
	gcc -c parse.c

display.o: display.c
	gcc -c display.c

username.o: username.c
	gcc -c username.c

clean:
	rm out