all:
	gcc -Wall -Werror -Wextra -pedantic -Isrc/include -Lsrc/lib -o main *.c -lmingw32 -lSDL2main -lSDL2
