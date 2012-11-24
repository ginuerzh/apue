#!/bin/sh

OBJ=zstat
UTILS=../utils
BINPATH=~/bin

${OBJ}:../utils.o ${OBJ}.o
	gcc ../utils.o ${OBJ}.o -o ${BINPATH}/${OBJ} 

${OBJ}.o: ${OBJ}.c
	gcc -Wall -c ${OBJ}.c

${UTILS}.o:${UTILS}.c
	gcc -Wall -c ${UTILS}.c -o ${UTILS}.o
clean:
	-rm -f *~ *.o ${UTILS}.o ${BINPATH}/${OBJ}
