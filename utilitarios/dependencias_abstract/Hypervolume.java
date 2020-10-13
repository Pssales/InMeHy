package dependencias_abstract;

import java.io.FileNotFoundException;
import java.util.List;

import dependencias_interfaces.Front;

/**
 * This interface represents implementations of the Hypervolume quality indicator

 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 * @author Juan J. Durillo
 */
@SuppressWarnings("serial")
public abstract class Hypervolume<S> extends GenericIndicator<S> {

  public Hypervolume() {
  }

  public Hypervolume(String referenceParetoFrontFile) throws FileNotFoundException {
    super(referenceParetoFrontFile);
  }

  public Hypervolume(Front referenceParetoFront) {
    super(referenceParetoFront);
  }

  public abstract List<S> computeHypervolumeContribution(List<S> solutionList, List<S> referenceFrontList) ;
  public abstract double getOffset() ;
  public abstract void setOffset(double offset) ;

  @Override public String getName() {
    return "HV" ;
  }

  @Override
  public boolean isTheLowerTheIndicatorValueTheBetter() {
    return false ;
  }
}
