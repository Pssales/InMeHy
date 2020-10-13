package algoritmos_evolucionarios.mombi;

import algoritmos_evolucionarios.AbstractMOMBI;
import algoritmos_evolucionarios.mombi.util.*;
import dependencias_interfaces.CrossoverOperator;
import dependencias_interfaces.MutationOperator;
import dependencias_interfaces.SelectionOperator;
import dependencias_interfaces.Problem;
import dependencias_interfaces.Solution;
import dependencias_interfaces.SolutionListEvaluator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

@SuppressWarnings("serial")
public class MOMBI<S extends Solution<?>> extends AbstractMOMBI<S>{

  protected final AbstractUtilityFunctionsSet<S> utilityFunctions;

  public MOMBI(
      Problem<S> problem,
      int maxIterations,
      CrossoverOperator<S> crossover,
      MutationOperator<S> mutation,
      SelectionOperator<List<S>, S> selection,
      SolutionListEvaluator<S> evaluator,
      String pathWeights) {
    super(problem, maxIterations, crossover, mutation, selection, evaluator);
    utilityFunctions = this.createUtilityFunction(pathWeights);
  }

  public AbstractUtilityFunctionsSet<S> createUtilityFunction(String pathWeights) {
    return new TchebycheffUtilityFunctionsSet<>(pathWeights, this.getReferencePoint());
  }

  public int getMaxPopulationSize() {
    return this.utilityFunctions.getSize();
  }

  @Override
  public void specificMOEAComputations() {
    updateNadirPoint(this.getPopulation());
    updateReferencePoint(this.getPopulation());
  }

  @Override
  protected List<S> replacement(List<S> population, List<S> offspringPopulation) {
    List<S> jointPopulation = new ArrayList<>();
    jointPopulation.addAll(population);
    jointPopulation.addAll(offspringPopulation);

    R2Ranking<S> ranking = computeRanking(jointPopulation);
    return selectBest(ranking);
  }

  protected R2Ranking<S> computeRanking(List<S> solutionList) {
    R2Ranking<S> ranking = new R2Ranking<>(this.utilityFunctions);
    ranking.computeRanking(solutionList);

    return ranking;
  }

  protected void addRankedSolutionsToPopulation(
      R2Ranking<S> ranking, int index, List<S> population) {
    for (S solution : ranking.getSubfront(index)) population.add(solution);
  }

  protected void addLastRankedSolutionsToPopulation(
      R2Ranking<S> ranking, int index, List<S> population) {
    List<S> front = ranking.getSubfront(index);
    Collections.sort(
        front,
        new Comparator<S>() {
          @Override
          public int compare(S arg0, S arg1) {
            R2RankingAttribute<S> attribute = new R2RankingAttribute<>();
            R2SolutionData dataFirst = attribute.getAttribute(arg0);
            R2SolutionData dataSecond = attribute.getAttribute(arg1);
            if (dataFirst.utility > dataSecond.utility) return -1;
            else if (dataFirst.utility < dataSecond.utility) return 1;
            else return 0;
          }
        });
    int remain = this.getMaxPopulationSize() - population.size();
    for (S solution : front.subList(0, remain)) population.add(solution);
  }

  protected List<S> selectBest(R2Ranking<S> ranking) {
    List<S> population = new ArrayList<>(this.getMaxPopulationSize());
    int rankingIndex = 0;

    while (populationIsNotFull(population)) {
      if (subfrontFillsIntoThePopulation(ranking, rankingIndex, population)) {
        addRankedSolutionsToPopulation(ranking, rankingIndex, population);
        rankingIndex++;
      } else {
        addLastRankedSolutionsToPopulation(ranking, rankingIndex, population);
      }
    }
    return population;
  }

  private boolean subfrontFillsIntoThePopulation(
      R2Ranking<S> ranking, int index, List<S> population) {
    return (population.size() + ranking.getSubfront(index).size() < this.getMaxPopulationSize());
  }

  protected AbstractUtilityFunctionsSet<S> getUtilityFunctions() {
    return this.utilityFunctions;
  }

  @Override
  public String getName() {
    return "MOMBI";
  }

  @Override
  public String getDescription() {
    return "Many-Objective Metaheuristic Based on the R2 Indicator";
  }
}
