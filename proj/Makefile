PROG=proj

.PATH: ${.CURDIR}/controller/keyboard
.PATH: ${.CURDIR}/controller/mouse

SRCS = 	main.c utils.c timer.c keyboard.c KBC.c mouse.c sprite.c video.c  game.c spaceShip.c asteroid.c bullet.c

CFLAGS += -pedantic -D_LCOM_OPTIMIZED_ -O2

DPADD += ${LIBLCF}
LDADD += -Wall -llcf -lxpm -llm
.include <minix.lcom.mk>
