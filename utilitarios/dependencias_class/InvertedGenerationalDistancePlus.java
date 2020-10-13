package dependencias_class;



import java.io.FileNotFoundException;
import java.math.BigDecimal;
import java.math.MathContext;
import java.util.List;

import dependencias_abstract.GenericIndicator;
import dependencias_interfaces.Front;
import dependencias_interfaces.Solution;

/**
 * This class implements the inverted generational distance metric plust (IGD+)
 * Reference: Ishibuchi et al 2015, "A Study on Performance Evaluation Ability of a Modified
 * Inverted Generational Distance Indicator", GECCO 2015
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
@SuppressWarnings("serial")
public class InvertedGenerationalDistancePlus<S extends Solution<?>> extends GenericIndicator<S> {

	
	
	

	 public BigDecimal evaluateModificado(List<S> solutionList) {
		 
	    if (solutionList == null) {
	      throw new JMetalException("The pareto front approximation is null") ;
	    }

	    return invertedGenerationalDistancePlusModificado(new ArrayFront(solutionList), referenceParetoFront);
	  }

	  /**
	   * Returns the inverted generational distance plus value for a given front
	   *
	   * @param front The front
	   * @param referenceFront The reference pareto front
	   */
	  public BigDecimal invertedGenerationalDistancePlusModificado(Front front, Front referenceFront) {

	    double sum = 0.0;
	    for (int i = 0 ; i < referenceFront.getNumberOfPoints(); i++) {
	      sum += FrontUtils.distanceToClosestPoint(referenceFront.getPoint(i),
	          front, new DominanceDistance());
	    }

	    // STEP 4. Divide the sum by the maximum number of points of the reference Pareto front
	    BigDecimal retorno = new BigDecimal(sum / referenceFront.getNumberOfPoints(), MathContext.UNLIMITED);
	    return retorno;
	  }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
  /**
   * Default constructor
   */
  public InvertedGenerationalDistancePlus() {
  }

  /**
   * Constructor
   *
   * @param referenceParetoFrontFile
   */
  public InvertedGenerationalDistancePlus(String referenceParetoFrontFile) throws FileNotFoundException {
    super(referenceParetoFrontFile) ;
  }

  /**
   * Constructor
   *
   * @param referenceParetoFront
   * @throws FileNotFoundException
   */
  public InvertedGenerationalDistancePlus(Front referenceParetoFront) {
    super(referenceParetoFront) ;
  }

  /**
   * Evaluate() method
   * @param solutionList
   * @return
   */
  @Override public Double evaluate(List<S> solutionList) {
    if (solutionList == null) {
      throw new JMetalException("The pareto front approximation is null") ;
    }

    return invertedGenerationalDistancePlus(new ArrayFront(solutionList), referenceParetoFront);
  }

  /**
   * Returns the inverted generational distance plus value for a given front
   *
   * @param front The front
   * @param referenceFront The reference pareto front
   */
  public double invertedGenerationalDistancePlus(Front front, Front referenceFront) {

    double sum = 0.0;
    for (int i = 0 ; i < referenceFront.getNumberOfPoints(); i++) {
      sum += FrontUtils.distanceToClosestPoint(referenceFront.getPoint(i),
          front, new DominanceDistance());
    }

    // STEP 4. Divide the sum by the maximum number of points of the reference Pareto front
    return sum / referenceFront.getNumberOfPoints();
  }

  @Override public String getName() {
    return "IGD+" ;
  }

  @Override public String getDescription() {
    return "Inverted generational distance quality indicator plus" ;
  }

  @Override
  public boolean isTheLowerTheIndicatorValueTheBetter() {
    return true ;
  }
}
