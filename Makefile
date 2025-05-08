#----------------------------------------------------------
# Developer---Cole Carter
# Course------CS4223-01
# Project-----Checkpoint #8
# Due---------April 23, 2025
#
# This builds checkpoint #8. 
#----------------------------------------------------------

a.out: parser.y scanner.l main.c symbol.c
	bison -y -d parser.y
	flex scanner.l
	gcc lex.yy.c y.tab.c main.c symbol.c -lfl

clean:
	rm -f a.out lex.yy.c y.tab.c y.tab.h
