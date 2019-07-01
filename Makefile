all:
	gcc main.c -o user -O2 -Wno-unused-result
clean:
	rm ./user