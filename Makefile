all:
	gcc -c main.c -o main.o -ftrapv -g -Wall -Wextra -Werror 
	gcc -c table.c -o table.o -ftrapv -g -Wall -Wextra -Werror 
	gcc -c io.c -o io.o -ftrapv -g -Wall -Wextra -Werror 
	gcc main.o table.o io.o  -o bin -Wall -Wextra -Werror -ftrapv -g 
