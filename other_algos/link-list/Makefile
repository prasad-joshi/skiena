CFLAGS:=-ggdb -I.

all: test-sll

test-sll: test-sll.c sll.o
	gcc -o test-sll sll.o test-sll.c $(CFLAGS)

sll.o: sll.c
	gcc -c sll.c $(CFLAGS)

clean:
	rm -f test-sll sll.o
