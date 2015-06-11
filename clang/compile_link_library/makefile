hello:
	gcc hello.c -o hello
hello.i:
	gcc -E hello.c -o hello.i
hello.s:hello.i
	gcc -S hello.i -o hello.s
hello.o:hello.s
	gcc -c hello.s -o hello.o

SimpleSection.o:
	gcc -c SimpleSection.c

clean:
	rm -rf hello hello.i hello.s hello.o
	rm -rf SimpleSection
