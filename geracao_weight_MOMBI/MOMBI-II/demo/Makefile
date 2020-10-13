###############################################################
#  Makefile     Compilation file.                             #
#                                                             #
#  September 2014                                             #
###############################################################/


OBJS_NOMPI	= main_moea.o main_indicator.o main_ndset.o

LPATH	= ../lib
IPATH	= ../include
CC	= gcc
CFLAGS	= -Wall -O3 -L$(LPATH) -I$(IPATH)

all:			$(OBJS_NOMPI)

mpi:			$(OBJS_MPI)

main_moea.o:		main_moea.c
			$(CC) $(CFLAGS) main_moea.c -o main_moea.o -lemo -lm

main_indicator.o:	main_indicator.c
			$(CC) $(CFLAGS) main_indicator.c -o main_indicator.o -lemo -lm

main_ndset.o:		main_ndset.c
			$(CC) $(CFLAGS) main_ndset.c -o main_ndset.o -lemo -lm

clean:		
			rm -f $(OBJS_NOMPI)  $(OBJS_MPI)

