
scr= main.c displyt.c
para= -Wall;
main:$(scr)
	gcc -o $(scr)  $(para)
clean:
	-rm -rf main
	-rm -rf a.out
	-rm -rf *.o
	-rm -rf *~
