all: lib

clean:
	rm -rf *.o
	rm -rf *.so

lib:
	gcc -fPIC -c straterand.c -Wall
	gcc -shared straterand.o -o straterand.so -Wall