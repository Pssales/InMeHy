package utilidades;

import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import algoritmos_evolucionarios.IBEA_LLH_IntegerProblem;
import algoritmos_evolucionarios.MOMBI_LLH_IntegerProblem;
import algoritmos_evolucionarios.NSGAIII_LLH_IntegerProblem;
import algoritmos_evolucionarios.NSGAII_LLH_IntegerProblem;
import algoritmos_evolucionarios.mombi.MOMBI2_LLH_IntegerProblem;
import dependencias_class.Saida;
import dependencias_class.SolutionListUtils;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.Problem;
import problemas.Camila_problema;
import utilidades.Impressora;

public class auxFronteiraPareto {

	private List<List<IntegerSolution>> allpopNSGAIII;
	private List<List<IntegerSolution>> allpopMOMBI;
	private List<List<IntegerSolution>> allpopMOMBI2;
	private List<List<IntegerSolution>> allpopIBEA;
	private ArrayList<IntegerSolution> pfTrueKnown;
	private ArrayList<IntegerSolution> pfTrueKnownFinal;
	private List<IntegerSolution> popMOMBI;

	private int estudo_caso;
	private int trials;
	private String caminho_saida = "";
	private int maxTrials = 30;
	private int populationSize = 100;
	private int operador_crossover = 1;
	private int operador_mutacao = 1;
	private double crossoverProbability = 0.9;
	private double mutationProbability = 0.0125;
	private int numberValidations = 1000;  //////// esse e o unico que muda;
	private String weight_path = "C:\\Users\\camil\\eclipse-workspace\\jMetal\\resources\\weightVectorFiles\\mombi2\\weight_03D_12.sld";
	private static String caminho_projeto = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\";
	private static int numberArchievment = 100;


	public auxFronteiraPareto() {

		allpopNSGAIII = new ArrayList<List<IntegerSolution>>();
		allpopMOMBI = new ArrayList<List<IntegerSolution>>();
		allpopMOMBI2 = new ArrayList<List<IntegerSolution>>();
		allpopIBEA = new ArrayList<List<IntegerSolution>>();
		pfTrueKnown = new ArrayList<IntegerSolution>();	
		pfTrueKnownFinal = new ArrayList<IntegerSolution>();		
		popMOMBI = new LinkedList<IntegerSolution>();

	}



	public void gerarFPFinal(int e, int t) throws IOException {

		System.out.println("Gerando a Fronteira de Pareto - Integer Problem...");

		this.estudo_caso = e;
		this.trials = t;
		this.caminho_saida = "pareto_fronts\\fronteira_pareto_"+estudo_caso+".pf";

		for(int i=0; i<trials; i++) {
			this.gerarFronteiraParetoUnitaria();
			this.limparTudo();
		}
		System.out.println("Fim !");

	}


	public void gerarFronteiraParetoUnitaria () throws IOException {

		//obrigatoriamente tem que estar aqui
		Problem<IntegerSolution> problem = new Camila_problema(caminho_projeto+"dots/"+estudo_caso+".dot");

		System.out.println("Executando...");

		for(int trial = 0; trial < maxTrials; trial++){
			NSGAIII_LLH_IntegerProblem nsgaiii_nativo = new NSGAIII_LLH_IntegerProblem(problem, populationSize, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations);
			try {
				Saida popNSGAIII_nativo = nsgaiii_nativo.execute();						   
				List<IntegerSolution> popnd = SolutionListUtils.getNondominatedSolutions(popNSGAIII_nativo.getPopulacao_final());
				allpopNSGAIII.add(popnd); 
				pfTrueKnown.addAll(popnd); 
			} catch (Exception eee) {
				eee.printStackTrace();
			}

		} 
		System.out.println("NSGAIII");
		
		
		for(int trial = 0; trial < maxTrials; trial++){

			IBEA_LLH_IntegerProblem ibea_nativo = new IBEA_LLH_IntegerProblem(problem, populationSize, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations*populationSize, numberArchievment);
			try {
				Saida popIBEA_nativo = ibea_nativo.execute();							   
				List<IntegerSolution> popnd = SolutionListUtils.getNondominatedSolutions(popIBEA_nativo.getPopulacao_final());
				allpopIBEA.add(popnd); // terah as 30 pops do NSGA-II
				pfTrueKnown.addAll(popnd); // para gerar a PFTrueKnown 
			} catch (Exception eee) {
				eee.printStackTrace();
			}

		} 
		System.out.println("IBEA");

		for(int trial = 0; trial < maxTrials; trial++){
			MOMBI2_LLH_IntegerProblem mombi2_nativo = new MOMBI2_LLH_IntegerProblem(problem, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations, weight_path);
			try {
				Saida popMOMBI2_nativo = mombi2_nativo.execute();		   
				List<IntegerSolution> popnd= SolutionListUtils.getNondominatedSolutions(popMOMBI2_nativo.getPopulacao_final());
				allpopMOMBI2.add(popnd); // terah as 30 pops do NSGA-II
				pfTrueKnown.addAll(popnd); // para gerar a PFTrueKnown   
			} catch (Exception eee) {
				eee.printStackTrace();
			}
		} // #### END 30 TRIAL NSGA-II
		
		System.out.println("MOMBI II Nativo finalizado"); 


		

		pfTrueKnown = (ArrayList<IntegerSolution>) SolutionListUtils.getNondominatedSolutions(pfTrueKnown);
		String fronteira = "";
		pfTrueKnownFinal = (ArrayList<IntegerSolution>) SolutionListUtils.getNondominatedSolutions(pfTrueKnown);
		
		for(IntegerSolution elemento : pfTrueKnownFinal){
			if(!fronteira.contains(elemento.getObjective(0) + " " + elemento.getObjective(1)+ " " +elemento.getObjective(2))) {
				fronteira = fronteira + elemento.getObjective(0) + " " + elemento.getObjective(1)+ " " +elemento.getObjective(2)+"\n";	
			}
		}
		

		Impressora.getInstance().imprimirArquivo((caminho_saida), fronteira);

	}

	private void limparTudo() {

		allpopNSGAIII.clear();
		allpopMOMBI.clear(); 
		popMOMBI.clear();
		allpopIBEA.clear();
		pfTrueKnown.clear();

	}
}
