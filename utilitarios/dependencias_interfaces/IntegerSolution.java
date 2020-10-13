package dependencias_interfaces;

/**
 * Interface representing a integer solutions
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
public interface IntegerSolution extends Solution<Integer> {
  public Integer getLowerBound(int index) ;
  public Integer getUpperBound(int index) ;
}
