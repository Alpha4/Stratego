clean:
	rm -rf *.o
	rm -rf *.so

straterand.o:
	gcc -c -fPIC straterand.c -o straterand.o

straterand.so: straterand.o
	gcc -shared -fPIC straterand.o -o straterand.so