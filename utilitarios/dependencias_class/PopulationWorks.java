package dependencias_class;

import java.util.List;


import dependencias_interfaces.Ranking;
import dependencias_interfaces.Solution;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 *
 * @author vinicius
 */
public class PopulationWorks {

	
	/** como pegar a população non-dominated
	 *   public static <S extends Solution<?>> List<S> getNondominatedSolutions(List<S> solutionList) {
    		Ranking<S> ranking = new DominanceRanking<S>() ;
    		return ranking.computeRanking(solutionList).getSubfront(0);
  		}
	 * @param sol
	 * @return
	 */
	
	public static <S extends Solution<?>> List<S> getNondominatedSolutions(List<S> solutionList) {
	    Ranking<S> ranking = new DominanceRanking<S>() ;
	    return ranking.computeRanking(solutionList).getSubfront(0);
	  }
	
    //  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
    public static SolutionSet removeDominadas(SolutionSet result ) {
        
    	Ranking_Sol ranking = new Ranking_Sol(result); 
        result = ranking.getSubfront(0);

        boolean dominador, dominado; 
        double valor1 = 0;
        double valor2 = 0;

        for (int i = 0; i < (result.size() - 1); i++) {
            for (int j = (i + 1); j < result.size(); j++) {
                dominador = true;
                dominado = true;

                for (int k = 0; k < result.get(i).getNumberOfObjectives(); k++) {
                    valor1 = result.get(i).getObjective(k);
                    valor2 = result.get(j).getObjective(k);

                    if (valor1 > valor2 || dominador == false) {
                        dominador = false;
                    } else if (valor1 <= valor2) {
                        dominador = true;
                    }

                    if (valor2 > valor1 || dominado == false) {
                        dominado = false;
                    } else if (valor2 < valor1) {
                        dominado = true;
                    }
                }

                if (dominador) {
//                    System.out.println("--------------------------------------------");
//                    System.out.println("Solucao [" + i + "] domina a Solucao [" + j + "]");
//                    System.out.println("[" + i + "] " + result.get(i).toString());
//                    System.out.println("[" + j + "] " + result.get(j).toString());

                    result.remove(j);
                    j = j - 1;
                } else if (dominado) {
//                    System.out.println("--------------------------------------------");
//                    System.out.println("Solucao [" + j + "] domina a Solucao [" + i + "]");
//                    System.out.println("[" + i + "] " + result.get(i).toString());
//                    System.out.println("[" + j + "] " + result.get(j).toString());

                    result.remove(i);
                    j = i;
                }
            }
        }
      
        return result;
    }

    //  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --  --
    public static SolutionSet removeRepetidas(SolutionSet result) {
        String solucao;

        for (int i = 0; i < result.size() - 1; i++) {
        	
        	double valor1 = result.get(i).getObjective(0);
        	solucao = String.valueOf(valor1);
            
        	for (int j = i + 1; j < result.size(); j++) {
                double valor2 = result.get(j).getObjective(0);
                
                String sol = String.valueOf(valor2);
        		if (solucao.equals(sol)) {
        			//System.out.println("aquiiiiiiiiiiiiiiiiiiiiiii");
                    //System.out.println("--------------------------------------------");
                    //System.out.println("Solucao [" + i + "] eh igual [" + j + "]");
                    //System.out.println(result.get(i).getDecisionVariables()[0].toString());
                    //System.out.println(result.get(j).getDecisionVariables()[0].toString());

                    result.remove(j);
                }
            }
        }
      //  System.out.println("saii");
        return result;
    }
}