package algoritmos_evolucionarios;


import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;





import dependencias_interfaces.Algorithm;
import dependencias_interfaces.CrossoverOperator;

import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.MutationOperator;
import dependencias_interfaces.Problem;
import dependencias_interfaces.SelectionOperator;
import operadores.IntegerCDGMutation;
import operadores.IntegerPolynomialMutation;
import operadores.IntegerSBXCrossover;
import operadores.IntegerSimpleRandomMutation;
import operadores.NPointCrossover;
import utilidades.Impressora;
import dependencias_abstract.AbstractAlgorithmRunner;
import dependencias_class.AlgorithmRunner;
import dependencias_class.ArrayFront;
import dependencias_class.BinaryTournamentSelection;
import dependencias_class.FrontNormalizer;
import dependencias_class.FrontUtils;
import dependencias_class.PISAHypervolume;
import dependencias_class.PointSolution;
import dependencias_class.PopulationWorks;
import dependencias_class.RankingAndCrowdingDistanceComparator;
import dependencias_class.Saida;
import dependencias_class.SequentialSolutionListEvaluator;
import dependencias_class.SolutionSet;



/**
 * Class for configuring and running the NSGA-II algorithm to solve the bi-objective TSP
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */

public class NSGAII_LLH_IntegerProblem extends AbstractAlgorithmRunner {

	private Problem<IntegerSolution> problem;
	private Algorithm<List<IntegerSolution>> algorithm;
	private CrossoverOperator<IntegerSolution> crossover;
	private MutationOperator<IntegerSolution>  mutation;
	private SelectionOperator<List<IntegerSolution>, IntegerSolution> selection;
	 

	 private int populationSize = 0;
	 private int operador_crossover = 0;
	 private int operador_mutacao = 0;
	 private double crossoverProbability = 0.0;
	 private double mutationProbability = 0.0;
	 private int maxEvaluations = 0; 
	 
	 
	public NSGAII_LLH_IntegerProblem(Problem<IntegerSolution> problem, int _populationSize, double _crossoverProbability, double _mutationProbability, int _operador_crossover, int _operador_mutacao, int _numberValidations ) throws IOException{

		    this.problem = problem;
		    populationSize = _populationSize;
		    operador_crossover = _operador_crossover;
		    operador_mutacao = _operador_mutacao;
		    crossoverProbability = _crossoverProbability;
			mutationProbability = _mutationProbability;
			maxEvaluations = _numberValidations;
		    
	}
	
	

	public Saida execute() throws InterruptedException, IOException{

		Double crossoverDistributionIndex = 20.0 ;
		switch(operador_crossover){
			case(1):
				crossover = new IntegerSBXCrossover(crossoverProbability, crossoverDistributionIndex) ;
				break;
			case(2):
				crossover = new NPointCrossover(crossoverProbability,1) ;
				break;
			case(3):
				crossover = new NPointCrossover(crossoverProbability,2) ;
				break;
		}
			
		
	    Double mutationDistributionIndex = 20.0 ;
	    switch(operador_mutacao){
			case(1):
				mutation = new IntegerPolynomialMutation(mutationProbability, mutationDistributionIndex) ;
				break;
			case(2):
				mutation = new IntegerCDGMutation(mutationProbability, 0.5) ;
				break;
			case(3):
				mutation = new IntegerSimpleRandomMutation(mutationProbability) ;
				break;
		}

		  
		    selection = new BinaryTournamentSelection<IntegerSolution>(new RankingAndCrowdingDistanceComparator<IntegerSolution>());
		    
		    algorithm = new NSGAIIBuilder<IntegerSolution>(problem, crossover, mutation)
		        .setSelectionOperator(selection)
		        .setMaxEvaluations(maxEvaluations)
		        .setPopulationSize(populationSize)
		        .build() ;
		    
		    

		    AlgorithmRunner algorithmRunner = new AlgorithmRunner.Executor(algorithm)
		        .execute();
	
		    List<IntegerSolution> population = algorithm.getResult();
			
		    Saida saida = new Saida(population, 0.0, 0.0); 
			
			return saida;
			
	 }
	
	public void limparPopulacao() {
		algorithm.getResult().clear();
	}
}