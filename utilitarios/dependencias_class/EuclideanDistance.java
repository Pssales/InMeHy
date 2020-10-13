package dependencias_class;


import dependencias_interfaces.Point;
import dependencias_interfaces.PointDistance;

/**
 * Computes the Euclidean distance between two points
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
public class EuclideanDistance implements PointDistance {

  @Override
  public double compute(Point a, Point b) {
    if (a == null) {
      throw new JMetalException("The first point is null") ;
    } else if (b == null) {
      throw new JMetalException("The second point is null") ;
    } else if (a.getDimension() != b.getDimension()) {
      throw new JMetalException("The dimensions of the points are different: "
          + a.getDimension() + ", " + b.getDimension()) ;
    }

    double distance = 0.0;

    for (int i = 0; i < a.getDimension(); i++) {
      distance += Math.pow(a.getValue(i) - b.getValue(i), 2.0);
    }
    return Math.sqrt(distance);
  }
}
