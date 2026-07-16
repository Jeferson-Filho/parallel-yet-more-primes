FLAGS=-O3 -Wall
OUT=primetest-parallel

all: primetest

primetest: primetest.c
	$(CC) primetest.c $(FLAGS) -lm -o $(OUT)

clean:
	rm $(OUT)
