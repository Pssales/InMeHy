package algoritmos_evolucionarios.moead.jmetal5version;

import dependencias_class.JMetalException;
import dependencias_class.JMetalRandom;
import dependencias_interfaces.CrossoverOperator;
import dependencias_interfaces.DifferentialEvolutionCrossover;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.MutationOperator;
import dependencias_interfaces.Problem;

import java.util.ArrayList;
import java.util.List;

/**
 * Class implementing the MOEA/D-DRA algorithm described in :
 * Q. Zhang,  W. Liu,  and H Li, The Performance of a New Version of
 * MOEA/D on CEC09 Unconstrained MOP Test Instances, Working Report CES-491,
 * School of CS & EE, University of Essex, 02/2009
 *
 * @author Juan J. Durillo
 * @author Antonio J. Nebro
 * @version 1.0
 */
@SuppressWarnings("serial")
public class MOEADDRA extends AbstractMOEAD<IntegerSolution> {
  protected DifferentialEvolutionCrossover differentialEvolutionCrossover ;

  protected IntegerSolution[] savedValues;
  protected double[] utility;
  protected int[] frequency;

  JMetalRandom randomGenerator ;

  public MOEADDRA(Problem<IntegerSolution> problem, int populationSize, int resultPopulationSize, int maxEvaluations,
                  MutationOperator<IntegerSolution> mutation, CrossoverOperator<IntegerSolution> crossover, FunctionType functionType,
                  String dataDirectory, double neighborhoodSelectionProbability,
                  int maximumNumberOfReplacedSolutions, int neighborSize) {
    super(problem, populationSize, resultPopulationSize, maxEvaluations, crossover, mutation, functionType,
        dataDirectory, neighborhoodSelectionProbability, maximumNumberOfReplacedSolutions,
        neighborSize);

    differentialEvolutionCrossover = (DifferentialEvolutionCrossover)crossoverOperator ;

    savedValues = new IntegerSolution[populationSize];
    utility = new double[populationSize];
    frequency = new int[populationSize];
    for (int i = 0; i < utility.length; i++) {
      utility[i] = 1.0;
      frequency[i] = 0;
    }

    randomGenerator = JMetalRandom.getInstance() ;
  }

  @Override public void run() {
    initializePopulation() ;
    initializeUniformWeight();
    initializeNeighborhood();
    idealPoint.update(population); ;

    int generation = 0 ;
    evaluations = populationSize ;
    do {
      List<Integer> order = tourSelection(10) ;
      for (int i = 0; i < order.size(); i++) {
        int subProblemId = order.get(i);
        frequency[subProblemId]++;

        NeighborType neighborType = chooseNeighborType() ;
        List<IntegerSolution> parents = parentSelection(subProblemId, neighborType) ;

        differentialEvolutionCrossover.setCurrentSolution(population.get(subProblemId));
        List<IntegerSolution> children = differentialEvolutionCrossover.execute(parents);

        IntegerSolution child = children.get(0) ;
        mutationOperator.execute(child);
        problem.evaluate(child);

        evaluations++;

        idealPoint.update(child.getObjectives());
        updateNeighborhood(child, subProblemId, neighborType);
      }

      generation++;
      if (generation % 30 == 0) {
        utilityFunction();
      }

    } while (evaluations < maxEvaluations);

  }

  protected void initializePopulation() {
    for (int i = 0; i < populationSize; i++) {
    	IntegerSolution newSolution = (IntegerSolution)problem.createSolution();

      problem.evaluate(newSolution);
      population.add(newSolution);
      savedValues[i] = (IntegerSolution) newSolution.copy();
    }
  }

  public void utilityFunction() throws JMetalException {
    double f1, f2, uti, delta;
    for (int n = 0; n < populationSize; n++) {
      f1 = fitnessFunction(population.get(n), lambda[n]);
      f2 = fitnessFunction(savedValues[n], lambda[n]);
      delta = f2 - f1;
      if (delta > 0.001) {
        utility[n] = 1.0;
      } else {
        uti = (0.95 + (0.05 * delta / 0.001)) * utility[n];
        utility[n] = uti < 1.0 ? uti : 1.0;
      }
      savedValues[n] = (IntegerSolution) population.get(n).copy();
    }
  }

  public List<Integer> tourSelection(int depth) {
    List<Integer> selected = new ArrayList<Integer>();
    List<Integer> candidate = new ArrayList<Integer>();

    for (int k = 0; k < problem.getNumberOfObjectives(); k++) {
      // WARNING! HERE YOU HAVE TO USE THE WEIGHT PROVIDED BY QINGFU Et AL (NOT SORTED!!!!)
      selected.add(k);
    }

    for (int n = problem.getNumberOfObjectives(); n < populationSize; n++) {
      // set of unselected weights
      candidate.add(n);
    }

    while (selected.size() < (int) (populationSize / 5.0)) {
      int best_idd = (int) (randomGenerator.nextDouble() * candidate.size());
      int i2;
      int best_sub = candidate.get(best_idd);
      int s2;
      for (int i = 1; i < depth; i++) {
        i2 = (int) (randomGenerator.nextDouble() * candidate.size());
        s2 = candidate.get(i2);
        if (utility[s2] > utility[best_sub]) {
          best_idd = i2;
          best_sub = s2;
        }
      }
      selected.add(best_sub);
      candidate.remove(best_idd);
    }
    return selected;
  }

  @Override public String getName() {
    return "MOEADDRA" ;
  }

  @Override public String getDescription() {
    return "Multi-Objective Evolutionary Algorithm based on Decomposition. Version with Dynamic Resource Allocation" ;
  }
}
