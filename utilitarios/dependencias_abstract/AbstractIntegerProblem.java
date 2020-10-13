package dependencias_abstract;


import java.util.List;

import dependencias_class.DefaultIntegerSolution;
import dependencias_interfaces.IntegerProblem;
import dependencias_interfaces.IntegerSolution;

@SuppressWarnings("serial")
public abstract class AbstractIntegerProblem extends AbstractGenericProblem<IntegerSolution>
  implements IntegerProblem {

  private List<Integer> lowerLimit ;
  private List<Integer> upperLimit ;

  /* Getters */
	@Override
	public Integer getUpperBound(int index) {
		return upperLimit.get(index);
	}

	@Override
	public Integer getLowerBound(int index) {
		return lowerLimit.get(index);
	}

  /* Setters */
  protected void setLowerLimit(List<Integer> lowerLimit) {
    this.lowerLimit = lowerLimit;
  }

  protected void setUpperLimit(List<Integer> upperLimit) {
    this.upperLimit = upperLimit;
  }

  @Override
  public IntegerSolution createSolution() {
    return new DefaultIntegerSolution(this) ;
  }

}
