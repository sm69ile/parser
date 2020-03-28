AC_INIT([parser], [1.0], [info@automatx.de])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_CC
AM_PROG_CC_C_O

AC_CHECK_LIB(X11, main, LIBX11=X11, LIBX11=no, -lX11)
AC_SUBST(LIBX11)
AC_CHECK_LIB(Xt, main, LIBXt=Xt, LIBXt=no, -lXt)
AC_SUBST(LIBXt)
AC_CHECK_LIB(Xaw, main, LIBXaw=Xaw, AC_MSG_ERROR("Test"), -lXaw)
AC_SUBST(LIBXaw)
AC_CHECK_LIB(pthread, main, LIBpthread=pthread, LIBpthread=no, -lpthread)
AC_SUBST(LIBpthread)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
