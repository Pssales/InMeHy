package dependencias_abstract;


import dependencias_class.ArrayFront;
import dependencias_class.JMetalException;
import dependencias_class.SimpleDescribedEntity;
import dependencias_interfaces.Front;
import dependencias_interfaces.QualityIndicator;

import java.io.FileNotFoundException;
import java.util.List;

/**
 * Abstract class representing quality indicators that need a reference front to be computed
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public abstract class GenericIndicator<S>
    extends SimpleDescribedEntity
    implements QualityIndicator<List<S>, Double> {

  protected Front referenceParetoFront = null ;

  /**
   * Default constructor
   */
  public GenericIndicator() {
  }

  public GenericIndicator(String referenceParetoFrontFile) throws FileNotFoundException {
    setReferenceParetoFront(referenceParetoFrontFile);
  }

  public GenericIndicator(Front referenceParetoFront) {
    if (referenceParetoFront == null) {
      throw new JMetalException("The reference pareto front is null");
    }

    this.referenceParetoFront = referenceParetoFront ;
  }

  public void setReferenceParetoFront(String referenceParetoFrontFile) throws FileNotFoundException {
    if (referenceParetoFrontFile == null) {
      throw new JMetalException("The reference pareto front is null");
    }

    Front front = new ArrayFront(referenceParetoFrontFile);
    referenceParetoFront = front ;
  }

  public void setReferenceParetoFront(Front referenceFront) throws FileNotFoundException {
    if (referenceFront == null) {
      throw new JMetalException("The reference pareto front is null");
    }

    referenceParetoFront = referenceFront ;
  }

  /**
   * This method returns true if lower indicator values are preferred and false otherwise
   * @return
   */
  public abstract boolean isTheLowerTheIndicatorValueTheBetter() ;
}
