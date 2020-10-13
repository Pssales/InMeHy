

Generates a set of weight vectors by means of the simplex-lattice design method.
Creates a file in the output directory. This file should be specified in the 
configuration file of MOMBI-II (wfile).

For compilation:
  make clean
  make

For running the program:
  ./sld.o
  Syntax: ./sld.o dimension samples

Example:
  ./sld.o 2 100

  cat output/weight_02D_100.sld 
  # 101 2
  1.000000e+00 0.000000e+00 
  9.900000e-01 1.000000e-02 
  9.800000e-01 2.000000e-02 
  9.700000e-01 3.000000e-02 
  9.600000e-01 4.000000e-02 
  9.500000e-01 5.000000e-02 
  ...

C:\Users\camil\Desktop\Doutorado\geracao_weight_MOMBI\weights>gcc sld.c -o sld
C:\Users\camil\Desktop\Doutorado\geracao_weight_MOMBI\weights>sld 3 12
