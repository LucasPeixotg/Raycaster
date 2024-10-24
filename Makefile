CFLAGS = -Wall -Wextra -pedantic -Werror -Wvla -g
LDFLAGS = -lm -lSDL2


build: algebra.o gametime.o player.o linked_list.o section.o levels.o
	gcc build/*.o src/constants.h src/main.c $(CFLAGS) -o main.out $(LDFLAGS)

algebra.o: src/algebra.c src/algebra.h
	gcc $(CFLAGS) -c src/algebra.c -o build/algebra.o

gametime.o: src/gametime.c src/gametime.h
	gcc $(CFLAGS) -c src/gametime.c -o build/gametime.o

player.o: src/player.c src/player.h
	gcc $(CFLAGS) -c src/player.c -o build/player.o

linked_list.o: src/linked_list.c src/linked_list.h
	gcc $(CFLAGS) -c src/linked_list.c -o build/linked_list.o

section.o: src/section.c src/section.h
	gcc $(CFLAGS) -c src/section.c -o build/section.o

levels.o: src/levels.c src/levels.h
	gcc $(CFLAGS) -c src/levels.c -o build/levels.o

run: build
	./main.out

clean:
	rm main.out