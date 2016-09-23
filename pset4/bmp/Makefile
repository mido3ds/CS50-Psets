all: clean c w r

r: resize.c
	clang -Wall -Werror -ggdb3 -O0 -std=c11 -o resize resize.c

w: whodunit.c
	clang -Wall -Werror -ggdb3 -O0 -std=c11 -o whodunit whodunit.c

c: copy.c 
	clang -Wall -Werror -ggdb3 -O0 -std=c11 -o copy copy.c

clean:
	rm -rf *.o copy copy.dsym whodunit whodunit.dsym resize