package dependencias_class;

import java.util.List;
import dependencias_interfaces.Solution;


/**
 * Class representing a nadir point (minimization is assumed)
 *
 * @author Antonio J.Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public class NadirPoint extends ArrayPoint {

  public NadirPoint(int dimension) {
    super(dimension) ;
    for (int i = 0; i < dimension; i++) {
      point[i] = Double.NEGATIVE_INFINITY ;
    }
  }

  @Override
  public void update(double[] point) {
    if (point.length != this.point.length) {
      throw new JMetalException("The point to be update have a dimension of " + point.length + " "
          + "while the parameter point has a dimension of " + point.length) ;
    }

    for (int i = 0; i < point.length; i++) {
      if (this.point[i] < point[i]) {
        this.point[i] = point[i];
      }
    }
  }

  public void update(List<? extends Solution<?>> solutionList) {
    for (Solution<?> solution : solutionList) {
      update(solution.getObjectives()) ;
    }
  }
}
