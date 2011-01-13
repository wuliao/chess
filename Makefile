
scr= main.c displayl.c chessboard.c mouse_set.c
para= -Wall;
main:$(scr)
	gcc -o main $(scr)  $(para)
clean: 
	-rm -rf main
	-rm -rf a.out
	-rm -rf *.o
	-rm -rf *~
