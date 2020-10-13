package dependencias_interfaces;

import java.io.Serializable;
import java.util.Comparator;

public interface ConstraintViolationComparator<S> extends Comparator<S>, Serializable {
  public int compare(S solution1, S solution2);
}
