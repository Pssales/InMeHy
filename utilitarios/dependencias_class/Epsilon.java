package dependencias_class;


import java.io.FileNotFoundException;
import java.math.BigDecimal;
import java.math.MathContext;
import java.util.List;

import dependencias_abstract.GenericIndicator;
import dependencias_interfaces.Solution;
import dependencias_interfaces.Front;
import dependencias_interfaces.IntegerSolution;

/**
 * This class implements the unary epsilon additive indicator as proposed in E.
 * Zitzler, E. Thiele, L. Laummanns, M., Fonseca, C., and Grunert da Fonseca. V
 * (2003): Performance Assessment of Multiobjective Optimizers: An Analysis and
 * Review. The code is the a Java version of the original metric implementation
 * by Eckart Zitzler. It can be used also as a command line program just by
 * typing $java org.uma.jmetal.qualityindicator.impl.Epsilon <solutionFrontFile>
 * <trueFrontFile> <getNumberOfObjectives>
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 * @author Juan J. Durillo
 */
@SuppressWarnings("serial")
public class Epsilon<S extends Solution<?>> extends GenericIndicator<S> {

  /**
   * Default constructor
   */
  public Epsilon() {
  }

  
  
  
  public BigDecimal evaluateModificado(List<IntegerSolution> pop) {
		 
	    if (pop == null) {
	      throw new JMetalException("The pareto front approximation is null") ;
	    }

	    return epsilonModificado(new ArrayFront(pop), referenceParetoFront);  }

  
  
  private BigDecimal epsilonModificado(Front front, Front referenceFront) throws JMetalException {

	  double eps, epsJ = 0.0, epsK = 0.0, epsTemp;

	    int numberOfObjectives = front.getPointDimensions() ;

	    eps = Double.MIN_VALUE;
	    
	    for (int i = 0; i < referenceFront.getNumberOfPoints(); i++) {
	      for (int j = 0; j < front.getNumberOfPoints(); j++) {
	        for (int k = 0; k < numberOfObjectives; k++) {
	          epsTemp = front.getPoint(j).getValue(k)
	              - referenceFront.getPoint(i).getValue(k);
	          if (k == 0) {
	            epsK = epsTemp;
	          } else if (epsK < epsTemp) {
	            epsK = epsTemp;
	          }
	        }
	        if (j == 0) {
	          epsJ = epsK;
	        } else if (epsJ > epsK) {
	          epsJ = epsK;
	        }
	      }
	      if (i == 0) {
	        eps = epsJ;
	      } else if (eps < epsJ) {
	        eps = epsJ;
	      }
	    }
	
	    return new BigDecimal(eps);
	  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /**
   * Constructor
   *
   * @param referenceParetoFrontFile
   * @throws FileNotFoundException
   */
  public Epsilon(String referenceParetoFrontFile) throws FileNotFoundException {
    super(referenceParetoFrontFile) ;
  }

  /**
   * Constructor
   *
   * @param referenceParetoFront
   */
  public Epsilon(Front referenceParetoFront) {
    super(referenceParetoFront) ;
  }

  @Override
  public boolean isTheLowerTheIndicatorValueTheBetter() {
    return true ;
  }

  /**
   * Evaluate() method
   *
   * @param popFinal_
   * @return
   */
  
  
  
  public Double evaluatee(List<IntegerSolution> popFinal_) {
	    if (popFinal_ == null) {
	      throw new JMetalException("The pareto front approximation list is null") ;
	    }

	    return epsilon(new ArrayFront(popFinal_), referenceParetoFront);
	  }
  
  @Override public Double evaluate(List<S> solutionList) {
    if (solutionList == null) {
      throw new JMetalException("The pareto front approximation list is null") ;
    }

    return epsilon(new ArrayFront(solutionList), referenceParetoFront);
  }

  /**
   * Returns the value of the epsilon indicator.
   *
   * @param front Solution front
   * @param referenceFront Optimal Pareto front
   * @return the value of the epsilon indicator
   * @throws JMetalException
   */
  private double epsilon(Front front, Front referenceFront) throws JMetalException {

    double eps, epsJ = 0.0, epsK = 0.0, epsTemp;

    int numberOfObjectives = front.getPointDimensions() ;

    eps = Double.MIN_VALUE;

    for (int i = 0; i < referenceFront.getNumberOfPoints(); i++) {
      for (int j = 0; j < front.getNumberOfPoints(); j++) {
        for (int k = 0; k < numberOfObjectives; k++) {
          epsTemp = front.getPoint(j).getValue(k)
              - referenceFront.getPoint(i).getValue(k);
          if (k == 0) {
            epsK = epsTemp;
          } else if (epsK < epsTemp) {
            epsK = epsTemp;
          }
        }
        if (j == 0) {
          epsJ = epsK;
        } else if (epsJ > epsK) {
          epsJ = epsK;
        }
      }
      if (i == 0) {
        eps = epsJ;
      } else if (eps < epsJ) {
        eps = epsJ;
      }
    }
    return eps;
  }

  @Override public String getName() {
    return "EP" ;
  }

  @Override public String getDescription() {
    return "Additive Epsilon quality indicator" ;
  }





}
