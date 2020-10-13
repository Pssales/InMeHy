package dependencias_class;

import dependencias_interfaces.Solution;
import dependencias_interfaces.SolutionAttribute;


/**
 * Generic class for implementing {@link SolutionAttribute} classes. By default, the identifier
 * of a {@link SolutionAttribute} is the class object, but it can be set to a different value
 * when constructing an instance.
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public class GenericSolutionAttribute <S extends Solution<?>, V> implements SolutionAttribute<S, V>{
  private Object identifier;

  /**
   * Constructora
   */
  public GenericSolutionAttribute() {
    identifier = this.getClass() ;
  }

  /**
   * Constructor
   * @param id Attribute identifier
   */
  public GenericSolutionAttribute(Object id) {
    this.identifier = id ;
  }

 
  @SuppressWarnings("unchecked")
  @Override
  public V getAttribute(S solution1) {
	    return (V)solution1.getAttribute(getAttributeIdentifier());
	  }
  @Override
  public void setAttribute(S solution, V value) {
     solution.setAttribute(getAttributeIdentifier(), value);
  }

  @Override
  public Object getAttributeIdentifier() {
    return identifier;
  }
}