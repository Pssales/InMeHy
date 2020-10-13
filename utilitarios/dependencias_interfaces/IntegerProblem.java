package dependencias_interfaces;

/**
 * Interface representing integer problems
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
public interface IntegerProblem extends Problem<IntegerSolution> {
  public Integer getLowerBound(int index) ;
  public Integer getUpperBound(int index) ;
}
