# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([view3d], [1.2-sdl2], [vladimir@stupin.su])
AC_CONFIG_SRCDIR([main.cpp])
AM_INIT_AUTOMAKE([foreign -Wportability -Wall -Werror -Wsyntax])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h math.h stdio.h stdlib.h string.h GL/gl.h SDL2/SDL.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

AC_CHECK_LIB([m], [sqrt])
AC_CHECK_LIB([SDL2], [SDL_CreateWindow, SDL_GetError, SDL_GL_CreateContext, SDL_GL_MakeCurrent, SDL_GL_SetAttribute, SDL_GL_SwapWindow, SDL_Init, SDL_PollEvent, SDL_Quit])
AC_CHECK_LIB([GL], [glClear, glClearDepth, glColor3f, glDepthFunc, glDisable, glEnable, glFinish, glFrustum, glHint, glLightf, glLightfv, glLoadIdentity, glLoadMatrixf, glMatrixMode, glPolygonMode, glRotatef, glShadeModel, glTexEnvf, glTexParameterf, glTranslatef])

AC_OUTPUT
