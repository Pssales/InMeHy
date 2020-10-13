# Makefile: Archivo de compilacion.
#
# Seccion de Computacion - CINVESTAV
#
# Introduccion a la Computacion Evolutiva
# Profesor: Carlos Artemio Coello Coello
# Autor:    Raquel Hernandez Gomez
# Mayo 2012
#

OBJS	= parameter.c randKnuth.c list.c function.c wfg.c genetic.c error.c
CC	= gcc
CFLAGS	= -Wall -O2

test:	$(OBJS) parameter.h randKnuth.h list.h function.h wfg.h genetic.h error.h
	$(CC) $(CFLAGS) -o mombi $(OBJS) -lm 

