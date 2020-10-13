package dependencias_class.checking.exception;

public class ValueOutOfRangeException extends RuntimeException {
  public ValueOutOfRangeException(int value, int lowestValue, int highestValue) {
    super("The parameter " + value + " is not in the range (" + lowestValue + ", " + highestValue + ")") ;
  }

  public ValueOutOfRangeException(double value, double lowestValue, double highestValue) {
    super("The parameter " + value + " is not in the range (" + lowestValue + ", " + highestValue + ")") ;
  }
}
