
###############################################################
#  Makefile     Compilation file.                             #
#                                                             #
#  September 2014                                             #
###############################################################/

OBJS_BASE	= io.o parser.o list.o avl.o random.o numeric.o sort.o stat.o vector.o matrix.o common.o evop.o benchmark.o knapsack.o dominance.o niche.o utility.o hv.o hv_wfg.o indicator.o plot.o moea.o mombi2.o stop.o debug.o param.o 

LPATH	= ../lib
CC	= gcc
AR	= ar
PROG	= libemo.a 
CFLAGS	= -Wall -O3

all:			$(OBJS_BASE)
			$(AR) -cvr $(PROG) $(OBJS_BASE) $(OBJS_NOMPI)
			mv $(PROG) $(LPATH) 

io.o:			io.h io.c
			$(CC) $(CFLAGS) -c io.c

parser.o:		parser.h parser.c
			$(CC) $(CFLAGS) -c parser.c

param.o:		param.h param.c
			$(CC) $(CFLAGS) -c param.c

list.o:			list.h list.c
			$(CC) $(CFLAGS) -c list.c

avl.o:			avl.h avl.c
			$(CC) $(CFLAGS) -c avl.c

random.o:		random.h random.c
			$(CC) $(CFLAGS) -c random.c

numeric.o:		numeric.h  numeric.c
			$(CC) $(CFLAGS) -c numeric.c

sort.o:			sort.h  sort.c
			$(CC) $(CFLAGS) -c sort.c

stat.o:			stat.h  stat.c
			$(CC) $(CFLAGS) -c stat.c

vector.o:		vector.h  vector.c
			$(CC) $(CFLAGS) -c vector.c

matrix.o:		matrix.h  matrix.c
			$(CC) $(CFLAGS) -c matrix.c

common.o:		common.h common.c
			$(CC) $(CFLAGS) -c common.c

moea.o:			moea.h moea.c
			$(CC) $(CFLAGS) -c moea.c

evop.o:			evop.h evop.c
			$(CC) $(CFLAGS) -c evop.c

benchmark.o:		wfg.h benchmark.h benchmark.c numeric.h
			$(CC) $(CFLAGS) -c benchmark.c

knapsack.o:		knapsack.h knapsack.c
			$(CC) $(CFLAGS) -c knapsack.c

dominance.o:		dominance.h dominance.c list.h sort.h
			$(CC) $(CFLAGS) -c dominance.c

niche.o:		niche.h niche.c vector.h sort.h
			$(CC) $(CFLAGS) -c niche.c

utility.o:		utility.h utility.c
			$(CC) $(CFLAGS) -c utility.c

hv.o:			hv.h hv.c avl.h
			$(CC) $(CFLAGS) -c hv.c

indicator.o:		indicator.h indicator.c
			$(CC) $(CFLAGS) -c indicator.c

plot.o:			plot.h plot.c
			$(CC) $(CFLAGS) -c plot.c

debug.o:		debug.h debug.c
			$(CC) $(CFLAGS) -c debug.c

stop.o:			stop.h stop.c
			$(CC) $(CFLAGS) -c stop.c

mombi2.o:		mombi2.h mombi2.c
			$(CC) $(CFLAGS) -c mombi2.c

clean:	
	rm -f $(LPATH)/$(PROG)
	rm -f $(OBJS_BASE) $(OBJS_NOMPI) $(OBJS_MPI)
