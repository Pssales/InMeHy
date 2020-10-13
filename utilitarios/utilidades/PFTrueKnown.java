package utilidades;

import java.util.ArrayList;
import java.util.List;

import dependencias_class.ArrayFront;
import dependencias_class.FrontNormalizer;
import dependencias_class.SolutionListUtils;
import dependencias_interfaces.Front;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.Solution;

public class PFTrueKnown {


	private ArrayList<IntegerSolution> pftrueknow;
	private static PFTrueKnown uniqueInstance;
	private Front referenceFront; 
	private FrontNormalizer frontNormalizer; 
	private Front normalizedReferenceFront;  



	public static PFTrueKnown getInstance() {
		if(uniqueInstance==null) {
			uniqueInstance = new PFTrueKnown();
		}
		return uniqueInstance;
	}

	private PFTrueKnown() {
		pftrueknow = new ArrayList<IntegerSolution>();
	}

	public ArrayList<IntegerSolution> getPftrueknow() {
		return pftrueknow;
	}

	public void setPftrueknow(ArrayList<IntegerSolution> pftrueknow) {
		this.pftrueknow = pftrueknow;
	}

	public void addElemento(IntegerSolution e){
		pftrueknow.add(e);	
	}
	
	

	public Front getReferenceFront() {
		return referenceFront;
	}

	public void setReferenceFront(Front referenceFront) {
		this.referenceFront = referenceFront;
	}

	public FrontNormalizer getFrontNormalizer() {
		return frontNormalizer;
	}

	public void setFrontNormalizer(FrontNormalizer frontNormalizer) {
		this.frontNormalizer = frontNormalizer;
	}

	public Front getNormalizedReferenceFront() {
		return normalizedReferenceFront;
	}

	public void setNormalizedReferenceFront(Front normalizedReferenceFront) {
		this.normalizedReferenceFront = normalizedReferenceFront;
	}


	
	public void gerarParetoFront() {
		
		referenceFront = new ArrayFront(pftrueknow);
		frontNormalizer = new FrontNormalizer(referenceFront);
		normalizedReferenceFront = frontNormalizer.normalize(referenceFront); 
	}

	public void addPop(List<IntegerSolution> populacao_final) {
		
		ArrayList<IntegerSolution> pftrueknowAux = new ArrayList<IntegerSolution>(SolutionListUtils.getNondominatedSolutions(populacao_final));
		
		for(IntegerSolution elemento : pftrueknowAux) {
			
			if(pftrueknow.contains(elemento)==true) { 
				System.out.println("lalala");
				continue;
			}
			
			boolean sin = true; 
			
			for(int a=0; a<elemento.getNumberOfObjectives(); a++) {
				if(elemento.getObjective(a)==1.0 || elemento.getObjective(a)==0.0) sin = false;
			}
			if(sin==true) {
				pftrueknow.add(elemento);
				
			}
		}
		
		pftrueknowAux.clear();
		
		
	}
	
	

}
