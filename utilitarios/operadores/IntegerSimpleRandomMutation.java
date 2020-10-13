package operadores;



import dependencias_class.JMetalException;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.MutationOperator;
import dependencias_interfaces.RandomGenerator;
import dependencias_class.JMetalRandom;

/**
 * This class implements a random mutation operator for double solutions
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public class IntegerSimpleRandomMutation implements MutationOperator<IntegerSolution> {
	
  private double mutationProbability ;
  private RandomGenerator<Double> randomGenerator ;

  /**  Constructor */
  public IntegerSimpleRandomMutation(double probability) {
	  this(probability, () -> JMetalRandom.getInstance().nextDouble());
  }

  /**  Constructor */
  public IntegerSimpleRandomMutation(double probability, RandomGenerator<Double> randomGenerator) {
    if (probability < 0) {
      throw new JMetalException("Mutation probability is negative: " + mutationProbability) ;
    }

  	this.mutationProbability = probability ;
    this.randomGenerator = randomGenerator ;
  }

  /* Getters */
  public double getMutationProbability() {
    return mutationProbability;
  }

  /* Setters */
  public void setMutationProbability(double mutationProbability) {
    this.mutationProbability = mutationProbability;
  }

  /** Execute() method */
	@Override
  public IntegerSolution execute(IntegerSolution solution) throws JMetalException {
    if (null == solution) {
      throw new JMetalException("Null parameter") ;
    }

    doMutation(mutationProbability, solution) ;
    
    return solution;
  }

  /** Implements the mutation operation */
	private void doMutation(double probability, IntegerSolution solution) {
		
    for (int i = 0; i < solution.getNumberOfVariables(); i++) {
      if (randomGenerator.getRandomValue() <= probability) {
      	double value = solution.getLowerBound(i) +
      			((solution.getUpperBound(i) - solution.getLowerBound(i)) * randomGenerator.getRandomValue()) ;
      	
      	solution.setVariableValue(i, (int) value);
      }
    }
	}
}
