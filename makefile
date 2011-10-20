decimal:
	gcc -g -o decimal decimal.c main.c

clean:
	rm decimal

style:
	astyle -n --style=linux decimal.c decimal.h main.c
