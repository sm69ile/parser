LIBX11 = @LIBX11@
LIBXt = @LIBXt@
LIBXaw = @LIBXaw@
LIBpthread = @LIBpthread@
LIBreadline = @LIBreadline@
SED = @SED@

bin_PROGRAMS = parser

parser_SOURCES = parser.tab.h parser.tab.c parser.c viewer.c viewer.h
parser_CFLAGS = -Wall -Wpedantic -DETCDIR=\"$(etcdir)\"
parser_LDADD = -l$(LIBXaw) -l$(LIBXt) -l$(LIBX11) -l$(LIBpthread) -l$(LIBreadline)

top_srcdir = @top_srcdir@

appdir = $(datadir)/$(PACKAGE)/app-defaults
dist_app_DATA = $(top_srcdir)/app-defaults/Viewer $(top_srcdir)/app-defaults/ax.xbm

symboldir = $(datadir)/$(PACKAGE)/symbols
dist_symbol_DATA = $(top_srcdir)/sym/*

etcdir = $(datadir)/$(PACKAGE)/etc
dist_etc_DATA = $(top_srcdir)/etc/*

AM_YFLAGS_PARSER = -b parser -d
AM_LFLAGS_PARSER = -o parser.c

dist_EXTRA_parser_SOURCES = parser.l parser.y parser.h

parser.tab.h: parser.y parser.h
	$(YACC) $(AM_YFLAGS_PARSER) parser.y

parser.tab.c: parser.y parser.h
	$(YACC) $(AM_YFLAGS_PARSER) parser.y

parser.c: parser.l
	$(LEX) $(AM_LFLAGS_PARSER) parser.l

install-data-hook:
	chmod -R u+w $(top_srcdir)
	$(SED) "s|__APPDEFAULTSDIR__|$(appdir)|" $(top_srcdir)/app-defaults/Viewer >$(appdir)/Viewer

clean-local:
	rm -f parser *.o a.out parser.tab.c parser.tab.h parser.c
