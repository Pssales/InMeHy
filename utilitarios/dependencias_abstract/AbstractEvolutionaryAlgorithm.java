package dependencias_abstract;


import java.util.List;
import dependencias_interfaces.Algorithm;
import dependencias_interfaces.Problem;



/**
 * Abstract class representing an evolutionary algorithm
 * @param <S> Solution
 * @param <R> Result
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public abstract class AbstractEvolutionaryAlgorithm<S, R>  implements Algorithm<R>{
  protected List<S> population;
  protected Problem<S> problem ;

  public List<S> getPopulation() {
    return population;
  }
  public void setPopulation(List<S> population) {
    this.population = population;
  }

  public void setProblem(Problem<S> problem2) {
    this.problem = problem2 ;
  }
  public Problem<S> getProblem() {
    return problem ;
  }

  protected abstract void initProgress();

  protected abstract void updateProgress();

  protected abstract boolean isStoppingConditionReached();

  protected abstract  List<S> createInitialPopulation() ;

  protected abstract List<S> evaluatePopulation(List<S> population);

  protected abstract List<S> selection(List<S> population);

  protected abstract List<S> reproduction(List<S> population);

  protected abstract List<S> replacement(List<S> population, List<S> offspringPopulation);

  @Override public abstract R getResult();

  @Override public void run() {
	  
    List<S> offspringPopulation;
    List<S> matingPopulation;
    
   
   // System.out.println("Criando população inicial");
    population = createInitialPopulation();
   
  
   // System.out.println("Validando população inicial");
    population = evaluatePopulation(population);
   // System.out.println("size: "+population.size());
  //  System.out.println("Iniciando o progresso");
    initProgress();
    
    //System.out.println("Entrando no loop");
    while (!isStoppingConditionReached()) {
      matingPopulation = selection(population);
      offspringPopulation = reproduction(matingPopulation);
      offspringPopulation = evaluatePopulation(offspringPopulation);
      population = replacement(population, offspringPopulation);
      updateProgress();
    }
   // System.out.println("size: "+population.size());
  //  System.out.println("Saindo do loop");
  }
}
