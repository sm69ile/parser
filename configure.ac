AC_INIT([parser], [1.0], [info@automatx.de])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_SED
AC_PROG_CC
AM_PROG_CC_C_O


AC_CHECK_LIB(X11, main, LIBX11=X11, AC_MSG_ERROR("libX11 not found"), -lX11)
AC_SUBST(LIBX11)
AC_CHECK_LIB(Xt, main, LIBXt=Xt, AC_MSG_ERROR("libXt not found"), -lXt)
AC_SUBST(LIBXt)
AC_CHECK_LIB(Xaw, main, LIBXaw=Xaw, AC_MSG_ERROR("libXaw not found"), -lXaw)
AC_SUBST(LIBXaw)
AC_CHECK_LIB(pthread, main, LIBpthread=pthread, AC_MSG_ERROR("libpthread not found"), -lpthread)
AC_SUBST(LIBpthread)
AC_CHECK_LIB(readline, main, LIBreadline=readline, AC_MSG_ERROR("libreadline not found"), -lreadline)
AC_SUBST(LIBreadline)


AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
