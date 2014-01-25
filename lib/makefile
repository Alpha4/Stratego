clean:
	rm -rf *.o
	rm -rf *.so

lib:
	gcc -c -fPIC straterand.c -o straterand.o
	gcc -shared -fPIC straterand.o -o straterand.so