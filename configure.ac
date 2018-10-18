AC_INIT([Hex-a-hop], [1.1.0], [http://hexahop.sourceforge.net], [hex-a-hop])
AC_CONFIG_AUX_DIR([msc])
AC_CONFIG_MACRO_DIR([msc/m4])
AM_INIT_AUTOMAKE(AC_PACKAGE_TARNAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(src/config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_STDC
AC_HEADER_STDC

AC_CHECK_HEADERS(stdint.h windows.h)
AC_CHECK_LIB(m, main)

dnl Check for SDL.
SDL_VERSION=1.2.0
AM_PATH_SDL($SDL_VERSION, :, AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!]))

dnl Arguments.
AC_ARG_ENABLE([debug], AC_HELP_STRING([--disable-debug], [Do not compile with debugging symbols]), [want_debug=no], [want_debug=yes])
AC_ARG_ENABLE([sound], AC_HELP_STRING([--disable-sound], [Do not compile with sound support]), [want_sound=no], [want_sound=yes])
AC_ARG_ENABLE([sdlttf], AC_HELP_STRING([--disable-sdlttf], [Use SDL_Pango instead of SDL_ttf]), [want_ttf=no], [want_ttf=yes])
AC_ARG_ENABLE([relpath], AC_HELP_STRING([--enable-relpath], [Install everything under a single directory]), [want_relative=yes], [want_relative=no])

dnl Compilation flags.
if test "$want_debug" = yes; then
	CXXFLAGS="$CXXFLAGS -g -Wall"
else
	CXXFLAGS="$CXXFLAGS -DNDEBUG -Wall"
fi

dnl Check for gettext.
AC_CHECK_HEADERS([libintl.h], [CXXFLAGS="$CXXFLAGS -DUSE_GETTEXT"])

dnl Conditional sound compilation.
if test "$want_sound" = yes; then
	MAYBE_SOUND="sound"

	dnl Check for SDL_mixer.
	SDL_LIBS="$SDL_LIBS -lSDL_mixer"
	SDL_CFLAGS="$SDL_CFLAGS"
else
	MAYBE_SOUND=
	CXXFLAGS="$CXXFLAGS -DDISABLE_SOUND"
fi

dnl Conditional Pango/TTF support.
if test "$want_ttf" = yes; then
	dnl Check for SDL_ttf.
	SDL_LIBS="$SDL_LIBS -lSDL_ttf"
	SDL_CFLAGS="$SDL_CFLAGS"
else
	dnl Check for SDL_Pango.
	SDL_LIBS="$SDL_LIBS -lSDL_Pango"
	SDL_CFLAGS="$SDL_CFLAGS"
	CXXFLAGS="$CXXFLAGS -DENABLE_PANGO"
fi

dnl Relative paths.
if test "$want_relative" = yes; then
	DATADIR="${prefix}/data"
	bindir="${prefix}"
	CXXFLAGS="$CXXFLAGS -DRELATIVE_PATHS"
	LDFLAGS="-Wl,-rpath -Wl,\\\$\$ORIGIN/lib $LDFLAGS"
else
	DATADIR="${datadir}/${PACKAGE}"
	CXXFLAGS="$CXXFLAGS -DDATADIR=\\\"\"${datadir}/${PACKAGE}\"\\\""
fi

dnl Export configuration.
AC_SUBST([CXXFLAGS])
AC_SUBST([DATADIR])
AC_SUBST([MAYBE_SOUND])
AC_SUBST([SDL_LIBS])
AC_SUBST([SDL_CFLAGS])

AC_OUTPUT([
Makefile
data/Makefile
src/Makefile
])
