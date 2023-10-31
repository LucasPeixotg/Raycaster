build:
	gcc src/*.c -Wall -Wextra -pedantic -std=c99 -lm -lSDL2 -o main.out

run:
	./main.out

clean:
	rm main.out