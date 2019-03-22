all:
	gcc main.c -o main.o

.PHONY: clean
clean:
	rm *.o