all:
	$(MAKE) -C src
	$(MAKE) -C demo

mpi:
	$(MAKE) -C src mpi
	$(MAKE) -C demo mpi

todo:	clean all mpi

clean:
	$(MAKE) -C src clean
	$(MAKE) -C demo clean

