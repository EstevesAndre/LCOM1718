# Makefile for proj

COMPILER_TYPE= gnu

CC= gcc

PROG= proj
SRCS= proj.c timer.c kbd.c mouse.c read_xpm.c sprite.c vbe.c video_gr.c rtc.c linked_list.c menu.c high_score.c game.c
SRCS+= mouse_sprite.c

CFLAGS= -Wall

DPADD+= ${LIBLM} ${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>