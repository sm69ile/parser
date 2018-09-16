AM_YFLAGS = -d
BUILT_SOURCES = y.tab.h lex.yy.c

bin_PROGRAMS = parser
parser_SOURCES = y.tab.c lex.yy.c
parser_CFLAGS = -Wall -g -O2

y.tab.h:	parser.y
	$(YACC) $(AM_YFLAGS) parser.y

lex.yy.c: parser.l y.tab.h
	$(LEX) parser.l

clean-local:
	rm -f parser .a.out lex.yy.c y.tab.c y.tab.h
