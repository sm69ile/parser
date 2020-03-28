LIBX11 = @LIBX11@
LIBXt = @LIBXt@
LIBXaw = @LIBXaw@
LIBpthread = @LIBpthread@

bin_PROGRAMS = parser
parser_SOURCES = y.tab.c lex.yy.c viewer.c viewer.h

#parser_CFLAGS = -Wall -Wpedantic -DDATADIR=\"$(datadir)\"

parser_CFLAGS = -Wall -Wpedantic -DETCDIR=\"$(etcdir)\"
parser_LDADD = -l$(LIBXaw) -l$(LIBXt) -l$(LIBX11) -l$(LIBpthread)

appdir = $(datadir)/$(PACKAGE)/app-defaults
app_DATA = ../app-defaults/Viewer

symboldir = $(datadir)/$(PACKAGE)/symbols
symbol_DATA = ../sym/*

etcdir = $(datadir)/$(PACKAGE)/etc
etc_DATA = ../etc/help.txt


AM_YFLAGS = -d

y.tab.c: parser.y parser.h
	$(YACC) $(AM_YFLAGS) parser.y

lex.yy.c: parser.l
	$(LEX) parser.l

clean-local:
	rm -f parser *.o  a.out lex.yy.c y.tab.c y.tab.h


