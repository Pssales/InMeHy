
Minimum requirements:

  gcc 4.1.2 
  MPI 3.0, MPICH 2.2
  GNU Make 3.81

For compilation:
  make clean
  make

The configuration files are in demo/input
The output files are generated in demo/output

For running the program:
  cd demo

  ./main_moea.o
  Syntax: ./main_moea.o parameter_file test_problem runs

Example:
./main_moea.o input/Param_03D.cfg dtlz1 1


In order to define a new Multi-objective Optimization Problem (MOP) 
edit the file demo/main_moea.c, and update the functions myMOP_eval and myMOP_alloc.

Re-compile and run the program, setting the test_problem as 'default'

Example:
./main_moea.o input/Param_02D.cfg default 1


