bin_PROGRAMS = parser
parser_SOURCES = y.tab.c lex.yy.c viewer.c viewer.h
parser_CFLAGS = -Wall -g -O2 -lX11 -lXt -lXaw -lpthread
AM_YFLAGS = -d

y.tab.c: parser.y parser.h
	$(YACC) $(AM_YFLAGS) parser.y

lex.yy.c: parser.l
	$(LEX) parser.l

clean-local:
	rm -f parser *.o  a.out lex.yy.c y.tab.c y.tab.h

