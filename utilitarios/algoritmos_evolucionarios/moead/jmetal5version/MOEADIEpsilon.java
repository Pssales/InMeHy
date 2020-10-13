package algoritmos_evolucionarios.moead.jmetal5version;

import algoritmos_evolucionarios.moead.jmetal5version.util.MOEADUtils;
import dependencias_class.ConstraintHandling;
import dependencias_class.CrowdingDistance;
//import dependencias_class.CrowdingDistance;
import dependencias_class.CrowdingDistanceComparator;
import dependencias_interfaces.Ranking;
import dependencias_class.DominanceRanking;
import dependencias_interfaces.Problem;
import dependencias_interfaces.CrossoverOperator;
import dependencias_interfaces.DifferentialEvolutionCrossover;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.MutationOperator;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static dependencias_class.ConstraintHandling.feasibilityRatio;
import static dependencias_class.ConstraintHandling.isFeasible;

/**
 * This class implements the MOEA/D-IEpsilon algorithm based on the one presented in the paper: "Z.
 * Fan, W. Li, X. Cai, H. Huang, Y. Fang, Y. You, J. Mo, C Wei, and E. D. Goodman, â€œAn improved
 * epsilon constraint-handling method in MOEA/D for CMOPs with large infeasible regions,â€� Soft
 * Computing, https://doi.org/10.1007/s00500-019-03794-x
 *
 * @author Antonio J. Nebro
 * @version 1.0
 */
@SuppressWarnings("serial")
public class MOEADIEpsilon extends AbstractMOEAD<IntegerSolution> {

  private DifferentialEvolutionCrossover differentialEvolutionCrossover;
  private double epsilonK;
  private double phiMax = -1e30;
  private List<IntegerSolution> archive ;

  public MOEADIEpsilon(
      Problem<IntegerSolution> problem,
      int populationSize,
      int resultPopulationSize,
      int maxEvaluations,
      MutationOperator<IntegerSolution> mutation,
      CrossoverOperator<IntegerSolution> crossover,
      FunctionType functionType,
      String dataDirectory,
      double neighborhoodSelectionProbability,
      int maximumNumberOfReplacedSolutions,
      int neighborSize) {
    super(
        problem,
        populationSize,
        resultPopulationSize,
        maxEvaluations,
        crossover,
        mutation,
        functionType,
        dataDirectory,
        neighborhoodSelectionProbability,
        maximumNumberOfReplacedSolutions,
        neighborSize);

    differentialEvolutionCrossover = (DifferentialEvolutionCrossover) crossoverOperator;
    archive = new ArrayList<>() ;
  }

  @Override
  public void run() {
    initializeUniformWeight();
    initializeNeighborhood();
    initializePopulation();
    idealPoint.update(population);

    double[] constraints = new double[populationSize];
    for (int i = 0; i < populationSize; i++) {
      constraints[i] = ConstraintHandling.overallConstraintViolationDegree(population.get(i));
      }
    Arrays.sort(constraints);
    double epsilonZero = Math.abs(constraints[(int) Math.ceil(0.05 * populationSize)]);

    if (phiMax < Math.abs(constraints[0])) {
      phiMax = Math.abs(constraints[0]);
    }

    int tc = (int) (0.8 * maxEvaluations / populationSize);
    tc = 800 ;
    double tao = 0.1;
    double rk = feasibilityRatio(population);

    evaluations = populationSize;
    int generationCounter = 0 ;
    epsilonK = epsilonZero;
    do {
      // Update the epsilon level
      if (generationCounter >= tc) {
        epsilonK = 0;
      } else {
        if (rk < 0.95) {
          epsilonK = (1 - tao) * epsilonK;
        } else {
          epsilonK = phiMax * (1 + tao);
        }
      }

      int[] permutation = new int[populationSize];
      MOEADUtils.randomPermutation(permutation, populationSize);

      for (int i = 0; i < populationSize; i++) {
        int subProblemId = permutation[i];

        NeighborType neighborType = chooseNeighborType();
        List<IntegerSolution> parents = parentSelection(subProblemId, neighborType);

        differentialEvolutionCrossover.setCurrentSolution(population.get(subProblemId));
        List<IntegerSolution> children = differentialEvolutionCrossover.execute(parents);

        IntegerSolution child = children.get(0);
        mutationOperator.execute(child);
        problem.evaluate(child);

        evaluations++;

        // Update PhiMax
        if (phiMax < Math.abs((double) ConstraintHandling.overallConstraintViolationDegree(child))) {
          phiMax = (double) ConstraintHandling.overallConstraintViolationDegree(child);
        }

        idealPoint.update(child.getObjectives());
        updateNeighborhood(child, subProblemId, neighborType);
      }
      rk = feasibilityRatio(population);

      updateExternalArchive();
      generationCounter++ ;
    } while (evaluations < maxEvaluations);
  }

  public void initializePopulation() {
    for (int i = 0; i < populationSize; i++) {
      IntegerSolution newSolution = (IntegerSolution) problem.createSolution();

      problem.evaluate(newSolution);
      population.add(newSolution);
    }
  }

  @Override
  protected void updateNeighborhood(
      IntegerSolution individual, int subproblemId, NeighborType neighborType) {
    int size;
    int numberOfReplaceSolutions;

    numberOfReplaceSolutions = 0;

    if (neighborType == NeighborType.NEIGHBOR) {
      size = neighborhood[subproblemId].length;
    } else {
      size = population.size();
    }
    int[] perm = new int[size];

    MOEADUtils.randomPermutation(perm, size);

    for (int i = 0; i < size; i++) {
      int k;
      if (neighborType == NeighborType.NEIGHBOR) {
        k = neighborhood[subproblemId][perm[i]];
      } else {
        k = perm[i];
      }

      double f1, f2;
      f1 = fitnessFunction(population.get(k), lambda[k]);
      f2 = fitnessFunction(individual, lambda[k]);

      double cons1 =
          Math.abs(ConstraintHandling.overallConstraintViolationDegree(population.get(k))) ;
      double cons2 =
          Math.abs(ConstraintHandling.overallConstraintViolationDegree(individual));

      if (cons1 < epsilonK && cons2 <= epsilonK) {
        if (f2 < f1) {
          population.set(k, (IntegerSolution) individual.copy());
          numberOfReplaceSolutions++;
        }
      } else if (cons1 == cons2) {
        if (f2 < f1) {
          population.set(k, (IntegerSolution) individual.copy());
          numberOfReplaceSolutions++;
        }
      } else if (cons2 < cons1) {
        population.set(k, (IntegerSolution) individual.copy());
        numberOfReplaceSolutions++;
      }

      if (numberOfReplaceSolutions >= maximumNumberOfReplacedSolutions) {
        return;
      }
    }
  }

  @Override
  public List<IntegerSolution> getResult() {
    return archive ;
  }

  @Override
  public String getName() {
    return "MOEA/D IEpsilon";
  }


  @Override
  public String getDescription() {
    return "MOEA/D with improved epsilon constraint handling method";
  }

  private void updateExternalArchive() {
    List<IntegerSolution> feasibleSolutions = new ArrayList<>() ;
    for (IntegerSolution solution: population) {
      if (isFeasible(solution)) {
        feasibleSolutions.add((IntegerSolution) solution.copy()) ;
      }
    }

    if (feasibleSolutions.size() > 0) {
      feasibleSolutions.addAll(archive) ;
      Ranking<IntegerSolution> ranking = new DominanceRanking<>() ;
      ranking.computeRanking(feasibleSolutions) ;

      List<IntegerSolution> firstRankSolutions = ranking.getSubfront(0) ;

      if (firstRankSolutions.size() <= populationSize) {
        archive.clear();
        for (IntegerSolution solution: firstRankSolutions) {
          archive.add((IntegerSolution)solution.copy()) ;
        }
      } else {
        CrowdingDistance<IntegerSolution> crowdingDistance = new CrowdingDistance<>() ;
        while (firstRankSolutions.size() > populationSize) {
          crowdingDistance.computeDensityEstimator(firstRankSolutions);
          firstRankSolutions.sort(new CrowdingDistanceComparator<>());
          firstRankSolutions.remove(firstRankSolutions.size() - 1) ;
        }

        archive.clear();
        for (int i = 0 ; i < populationSize; i++) {
          archive.add((IntegerSolution)firstRankSolutions.get(i).copy()) ;
        }
      }
    }
  }
}
