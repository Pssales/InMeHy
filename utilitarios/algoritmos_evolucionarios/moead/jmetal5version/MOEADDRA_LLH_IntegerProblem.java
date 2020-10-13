package algoritmos_evolucionarios.moead.jmetal5version;

import java.io.IOException;
import java.util.List;

import algoritmos_evolucionarios.moead.jmetal5version.AbstractMOEAD.FunctionType;
import dependencias_class.AlgorithmRunner;
import dependencias_class.BinaryTournamentSelection;
import dependencias_class.RankingAndCrowdingDistanceComparator;
import dependencias_class.Saida;
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

public class MOEADDRA_LLH_IntegerProblem {

	 private Problem<IntegerSolution> problem;
	 private Algorithm<List<IntegerSolution>> algorithm;
	 private CrossoverOperator<IntegerSolution> crossover;
	 private MutationOperator<IntegerSolution>  mutation;
	 private SelectionOperator<List<IntegerSolution>, IntegerSolution> selection;
	 
	 private int operador_crossover = 0;
	 private int operador_mutacao = 0;
	 private Double crossoverProbability = 0.0;
	 private Double mutationProbability = 0.0;
	 private int maxEvaluations = 0; 
	 
	 private int resultPopulationSize;
	 private int populationSize;
	 private int neighborSize;
	 private String dataDirectory;
	 private FunctionType functionType;
	 private double neighborhoodSelectionProbability;
	 private int maximumNumberOfReplacedSolutions;
	 
	 public MOEADDRA_LLH_IntegerProblem(Problem<IntegerSolution> problem, double _crossoverProbability, double _mutationProbability, int _operador_crossover, int _operador_mutacao, int _numberValidations, int resultPopulationSize, int populationSize, int neighborSize, String dataDirectory,	FunctionType functionType, double neighborhoodSelectionProbability, int maximumNumberOfReplacedSolutions) throws IOException{
		    
			this.problem = problem;
		    operador_crossover = _operador_crossover;
		    operador_mutacao = _operador_mutacao;
		    crossoverProbability = _crossoverProbability;
			mutationProbability = _mutationProbability;
			maxEvaluations = _numberValidations;
			this.populationSize = populationSize;
			this.neighborSize = neighborSize;
			this.dataDirectory = dataDirectory;
			this.functionType = functionType;
			this.neighborhoodSelectionProbability = neighborhoodSelectionProbability;
			this.maximumNumberOfReplacedSolutions = maximumNumberOfReplacedSolutions;
	}


	public Saida execute() throws IOException{
		 
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
		    
		 
			algorithm = new MOEADDRA(problem, populationSize, resultPopulationSize, maxEvaluations, mutation, crossover, functionType, dataDirectory, neighborhoodSelectionProbability, maximumNumberOfReplacedSolutions, neighborSize);
		    		    
		    AlgorithmRunner algorithmRunner = new AlgorithmRunner.Executor(algorithm)
			        .execute() ;
			    
			    List<IntegerSolution> population = algorithm.getResult() ;
		Saida saida = new Saida(population, 0.0, 0.0); 
		return saida;
	 }
	
	public void limparPopulacao() {
		algorithm.getResult().clear();
	}
}
