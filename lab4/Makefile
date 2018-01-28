# Makefile for lab4

COMPILER_TYPE= gnu

CC= gcc

PROG= lab4
SRCS= lab4.c test4.c mouse.c

CFLAGS= -Wall
CPPFLAGS += -D LAB4

DPADD+= ${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
