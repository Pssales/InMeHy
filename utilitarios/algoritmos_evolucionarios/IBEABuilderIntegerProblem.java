package algoritmos_evolucionarios;


import operadores.IntegerPolynomialMutation;
import operadores.IntegerSBXCrossover;

import java.util.List;

import dependencias_interfaces.AlgorithmBuilder;
import dependencias_interfaces.CrossoverOperator;
import dependencias_interfaces.MutationOperator;
import dependencias_interfaces.Problem;
import dependencias_interfaces.SelectionOperator;
import dependencias_interfaces.IntegerSolution;
import dependencias_class.BinaryTournamentSelection;


/**
 * This class implements the IBEA algorithm
 */
public class IBEABuilderIntegerProblem implements AlgorithmBuilder<IBEA<IntegerSolution>> {
	
  private Problem<IntegerSolution> problem;
  private int populationSize;
  private int archiveSize;
  private int maxEvaluations;

  private CrossoverOperator<IntegerSolution> crossover;
  private MutationOperator<IntegerSolution> mutation;
  private SelectionOperator<List<IntegerSolution>, IntegerSolution> selection;

  /**
   * Constructor
   * @param problem
   */
  public IBEABuilderIntegerProblem(Problem<IntegerSolution> problem) {
    this.problem = problem;
    populationSize = 100;
    archiveSize = 100;
    maxEvaluations = 25000;

    double crossoverProbability = 0.9;
    double crossoverDistributionIndex = 20.0;
    crossover = new IntegerSBXCrossover(crossoverProbability, crossoverDistributionIndex);

    double mutationProbability = 1.0 / problem.getNumberOfVariables();
    double mutationDistributionIndex = 20.0;
    mutation = new IntegerPolynomialMutation(mutationProbability, mutationDistributionIndex);

    selection = new BinaryTournamentSelection<IntegerSolution>();
  }

  /* Getters */
  public int getPopulationSize() {
    return populationSize;
  }

  public int getArchiveSize() {
    return archiveSize;
  }

  public int getMaxEvaluations() {
    return maxEvaluations;
  }

  public CrossoverOperator<IntegerSolution> getCrossover() {
    return crossover;
  }

  public MutationOperator<IntegerSolution> getMutation() {
    return mutation;
  }

  public SelectionOperator<List<IntegerSolution>, IntegerSolution> getSelection() {
    return selection;
  }

  /* Setters */
  public IBEABuilderIntegerProblem setPopulationSize(int populationSize) {
    this.populationSize = populationSize;

    return this;
  }

  public IBEABuilderIntegerProblem setArchiveSize(int archiveSize) {
    this.archiveSize = archiveSize;

    return this;
  }

  public IBEABuilderIntegerProblem setMaxEvaluations(int maxEvaluations) {
    this.maxEvaluations = maxEvaluations;

    return this;
  }

  public IBEABuilderIntegerProblem setCrossover(CrossoverOperator<IntegerSolution> crossover) {
    this.crossover = crossover;

    return this;
  }

  public IBEABuilderIntegerProblem setMutation(MutationOperator<IntegerSolution> mutation) {
    this.mutation = mutation;

    return this;
  }

  public IBEABuilderIntegerProblem setSelection(SelectionOperator<List<IntegerSolution>, IntegerSolution> selection) {
    this.selection = selection;

    return this;
  }

  public IBEA<IntegerSolution> build() {
    return new IBEA<IntegerSolution>(problem, populationSize, archiveSize, maxEvaluations, selection, crossover,
        mutation);
  }
}
