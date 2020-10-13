package dependencias_interfaces;

/**
 * Interface representing distances between two entities
 *
 * @author <antonio@lcc.uma.es>
 */
public interface Distance<E, J> {
  double getDistance(E element1, J element2) ;
}
