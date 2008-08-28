AC_INIT([sxc-tools], [0.0-prealpha], [sxc team])
AC_PREREQ(2.50)
AM_INIT_AUTOMAKE([-Wall -Werror])
AC_DEFINE_UNQUOTED(
    COPYRIGHT,
    ["2008 Andreas Waidler, Dennis Felsing"],
    [Copyright information.])

AC_CONFIG_SRCDIR([sxc-register/src/main.cxx])
AC_CONFIG_HEADER([config.hxx])

# cxxflags#{{{
AC_MSG_CHECKING([whether configure should try to set CXXFLAGS])
if test "x${CXXFLAGS+set}" = xset; then
    enable_cxxflags_setting=no
else
    enable_cxxflags_setting=yes
fi
AC_MSG_RESULT($enable_cxxflags_setting)
#}}}

# Optional Features
# debug#{{{
AC_ARG_ENABLE(
    [debug],
    AC_HELP_STRING(
        [--enable-debug],
        [enable debugging output to stderr @<:@default=no@>:@]),
    [debug=${enableval}],
    [debug=no])

if test "x${debug}" = xyes; then
    AC_DEFINE(
        DEBUG,
        [1],
        [Define to 1 to enable debugging output to stderr])
    if test "x${enable_cxxflags_setting}" = xyes; then
        CXXFLAGS="-g -O2" # Debugging information for gdb.
    fi
else
    if test "x${enable_cxxflags_setting}" = xyes; then
        CXXFLAGS="-O2"
    fi
fi
#}}}
# dot-multi-targets#{{{
AC_ARG_ENABLE(
    [dot-multi-targets],
    AC_HELP_STRING(
        [--disable-dot-multi-targets],
        [disable dot's output to multiple files in one run (make doxygen)
         @<:@default=use@:>@]),
    [dot_multi_targets=${withval}],
    [dot_multi_targets=yes])
#}}}

# Optional Packages
# dot#{{{
AC_ARG_WITH(
    [dot],
    AC_HELP_STRING(
        [--without-dot],
        [do not use dot for graph generation (make doxygen)
         @<:@default=use@:>@]),
    [with_dot=${withval}],
    [with_dot=yes])
#}}}
# htags#{{{
AC_ARG_WITH(
    [htags],
    AC_HELP_STRING(
        [--with-htags],
        [enable the htags (GNU GLOBAL) source browser (make doxygen)
         @<:@default=no@:>@]),
    [with_htags=${withval}],
    [with_htags=no])
#}}}

# Checks for programs.
# AC_PROG_*#{{{
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_PROG_AWK
AC_PROG_SED
#}}}
# doxygen#{{{
AC_CHECK_PROG([have_doxygen], [doxygen], [yes], [no])
if test "x${have_doxygen}" = xno; then
    echo \
"-------------------------------------------------
 Unable to find Doxygen.
 \"make doxygen\" will not work.
-------------------------------------------------"
fi
#}}}
# perl#{{{
AC_PATH_PROG([perl_path], [perl], [no])
if test "x${perl_path}" = xno; then
    echo \
"-------------------------------------------------
 Unable to find Perl.
 \"make doxygen\" will not work.
-------------------------------------------------"
fi
#}}}
if test "x${have_doxygen}" = xyes -a "x${perl_path}" != xno; then
    # dot#{{{
    if test "x${with_dot}" = xyes; then
        AC_CHECK_PROG([with_dot], [dot], [yes], [no])
        if test "x${with_dot}" = xno; then
            echo \
"-------------------------------------------------
 Unable to find dot.
 \"make doxygen\" will not build powerful graphs.
-------------------------------------------------"
        else
            if test "x${dot_multi_targets}" = xyes; then
                dot_version=`dot -V 2>&1 | \
                    sed -e 's/.*version //' | sed -e 's/ .*//'`
                # Requires AX_COMPARE_VERSION from the Autoconf Macro Archive
                AX_COMPARE_VERSION(
                    [$dot_version],
                    [gt],
                    [1.8.10],
                    [],
                    [dot_multi_targets=no])
                AC_MSG_CHECKING([for dot > 1.8.10])
                AC_MSG_RESULT([$dot_multi_targets])
                if test "x${dot_multi_targets}" = xno; then
                    echo \
"-------------------------------------------------
 Unable to find dot > 1.8.10.
 \"make doxygen\" will not enable dot to output
 multiple files in one run. Therefore it will run
 slower.
-------------------------------------------------"
                fi
            fi
        fi
    else
        dot_multi_targets=no
    fi
#}}}
    # htags#{{{
    if test "x${with_htags}" = xyes; then
        AC_MSG_CHECKING([for htags >= 4.8.6])
        htags_version=`htags --version |
            sed -ne 1p | sed -e 's/.* //'`
        # Requires AX_COMPARE_VERSION from the Autoconf Macro Archive
        AX_COMPARE_VERSION(
            [$htags_version],
            [ge],
            [4.8.6],
            [],
            [with_htags=no])
        AC_MSG_RESULT([$with_htags])
        if test "x${with_htags}" = xno; then
            echo \
"-------------------------------------------------
 Unable to find htags (GNU GLOBAL) >= 4.8.6.
 \"make doxygen\" will not use htags as the
 source browser.
-------------------------------------------------"
        fi
    fi
#}}}
else
    with_dot=no
    dot_multi_targets=no
    with_htags=no
fi

# Checks for language.
AC_LANG([C++])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h termios.h])

# Checks for libraries.
# libsxc#{{{
PKG_CHECK_MODULES([LIBSXC], [libsxc], [have_libsxc=yes], [have_libsxc=no])
if test "x${have_libsxc}" = xno; then
    echo \
"-------------------------------------------------
 Unable to find libsxc. Stopping
 Check \"config.log\" for more information.
-------------------------------------------------"
    (exit 1); exit 1
fi
#}}}
# gloox#{{{
PKG_CHECK_MODULES([GLOOX], [gloox >= 1.0], [have_gloox=yes], [have_gloox=no])
if test "x${have_gloox}" = xno; then
    echo \
"-------------------------------------------------
 Unable to find gloox >= 1.0. Stopping
 Check \"config.log\" for more information.
-------------------------------------------------"
    (exit 1); exit 1
fi
#}}}
# cppunit#{{{
PKG_CHECK_MODULES(
    [CPPUNIT],
    [cppunit >= 1.12.0],
    [have_cppunit=yes],
    [have_cppunit=no])
if test "x${have_cppunit}" = xno; then
    echo \
"-------------------------------------------------
 Unable to find CppUnit >= 1.12.0.
 \"make check\" will not work.
-------------------------------------------------"
fi
#}}}

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_STAT
AC_CHECK_FUNCS([strerror])

# Substs#{{{
AC_SUBST(debug)
AC_SUBST(dot_multi_targets)

AC_SUBST(perl_path)
AC_SUBST(with_dot)
AC_SUBST(with_htags)

AC_SUBST(LIBSXC_CFLAGS)
AC_SUBST(LIBSXC_LIBS)
AC_SUBST(GLOOX_CFLAGS)
AC_SUBST(GLOOX_LIBS)
AC_SUBST(CPPUNIT_CFLAGS)
AC_SUBST(CPPUNIT_LIBS)
#}}}

#AC_CONFIG_FILES([#{{{
AC_CONFIG_FILES([
    Doxyfile
    Makefile
    sxc-register/Makefile
    sxc-register/src/Makefile
])#}}}

AC_OUTPUT#{{{

echo \
"-------------------------------------------------
${srcdir}
 ${PACKAGE_NAME} ${PACKAGE_VERSION}

 Prefix: '${prefix}'.
 Compiler: '${CXX} ${CXXFLAGS} ${CPPFLAGS}'

 Package features:
  debug              : ${debug}
  make doxygen:
   doxygen (needed)  : ${have_doxygen}
   Perl (needed)     : ${perl_path}
   dot               : ${with_dot}
   dot multi targets : ${dot_multi_targets}
   htags             : ${with_htags}
  make check:
   cppunit (needed)  : ${have_cppunit}

 Now type 'make @<:@<target>@:>@'
 where the optional <target> is:
  all                - build all binaries
  check              - run unittests
  doxygen            - build the documentation
  install            - install everything
  uninstall          - uninstall everything

-------------------------------------------------"
#}}}
