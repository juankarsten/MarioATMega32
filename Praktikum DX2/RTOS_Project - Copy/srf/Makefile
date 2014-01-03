# $Id: Makefile,v 1.8 2007/08/02 01:24:42 bsd Exp $

TARGET = srf08

CPU = atmega128

SRCS = i2c.c main.c srf08.c

OBJS = i2c.o main.o srf08.o

OPTIMIZE = -O
CFLAGS   = -g -mmcu=${CPU} -Wall ${OPTIMIZE}
LDFLAGS  = -mmcu=${CPU} -Wl,-u,vfprintf -lprintf_flt -lm

CC     = avr-gcc

all : depend
	make ${TARGET}

.c.x :
	avr-gcc -c ${CFLAGS} -S -o $@ $<

${TARGET} : ${OBJS}
	avr-gcc -mmcu=${CPU} ${LDFLAGS} -o ${TARGET} ${OBJS}
	avr-objcopy -O ihex ${TARGET} ${TARGET}.hex
	avr-objcopy -O srec ${TARGET} ${TARGET}.srec
	avr-objcopy -O binary ${TARGET} ${TARGET}.bin
	@ls -l ${TARGET}.bin

clean :
	rm -f *.srec *.elf *.o *.bin *.s *~ .depend Makefile.bak

assembly :
	for i in ${OBJS}; do make $${i%%.*}.s; done

depend :
	@cp Makefile Makefile.bak
	@awk '/# .Id/,/^# DO NOT DELETE/' Makefile > Makefile.new
	@${CC} ${CFLAGS} -MM ${SRCS} >> Makefile.new
	@mv Makefile.new Makefile

# DO NOT DELETE (Dependencies follow)
i2c.o: i2c.c i2c.h
main.o: main.c i2c.h srf08.h
srf08.o: srf08.c i2c.h srf08.h
