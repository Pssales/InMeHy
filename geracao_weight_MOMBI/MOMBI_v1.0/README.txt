# Requirements:
gcc 4.6.1 or any later
gnuplot 4.4 or any later
gnu make 3.81 or any later

# Compiling:
make

# Execution of the algorithm:
./mombi param/file.config

# Example:
./mombi param/ZDT1.config

# The algorithm generates output files in the directory ./output

# Generation of weight vectors:
gcc -o pesos pesos.c
./pesos parametro_h dimension

# Folder organization:

weight: weight vectors for different dimensions
param:  configuration files
PFTrue: Pareto Front True (only if plot = 1 in the configuration file)
output: output files of MOMBI

MOMBI_problem_dimension_ejecution.pf  - Pareto front approximation
MOMBI_problem_dimension_ejecution.pos - Pareto optimal set
MOMBI_problema_dimension.sum          - Summary of the parameters
