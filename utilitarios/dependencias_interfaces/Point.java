package dependencias_interfaces;

/**
 * Interface representing a point
 *
 * @author Antonio J. Nebro
 */
public interface Point {
  int getDimension();
  double[] getValues() ;
  double getValue(int index) ;
  void setValue(int index, double value) ;
  void update(double[] point) ;
}
