CC = gcc
all: bin/main

bin/main: obj/main.o 
	$(CC) -o $@ $^

obj/main.o: src/main.c
	$(CC) $< -c -o $@ 

run: 
	@./bin/main

clean:
	rm -rf ./bin/main ./obj/*.o