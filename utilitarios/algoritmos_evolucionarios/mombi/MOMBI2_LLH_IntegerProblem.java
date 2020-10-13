package algoritmos_evolucionarios.mombi;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import dependencias_abstract.AbstractAlgorithmRunner;
import dependencias_class.AlgorithmRunner;
import dependencias_class.BinaryTournamentSelection;
import dependencias_class.RankingAndCrowdingDistanceComparator;
import dependencias_class.Saida;
import dependencias_class.SequentialSolutionListEvaluator;

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

import algoritmos_evolucionarios.mombi.MOMBI2;

public class MOMBI2_LLH_IntegerProblem extends AbstractAlgorithmRunner{

	 private Problem<IntegerSolution> problem;
	 private Algorithm<List<IntegerSolution>> algorithm;
	 private CrossoverOperator<IntegerSolution> crossover;
	 private MutationOperator<IntegerSolution>  mutation;
	 private SelectionOperator<List<IntegerSolution>, IntegerSolution> selection;
	 private String weight_path;
	 
	 private int operador_crossover = 0;
	 private int operador_mutacao = 0;
	 private Double crossoverProbability = 0.0;
	 private Double mutationProbability = 0.0;
	 private int maxEvaluations = 0; 
	 
	 public MOMBI2_LLH_IntegerProblem(Problem<IntegerSolution> problem, double _crossoverProbability, double _mutationProbability, int _operador_crossover, int _operador_mutacao, int _numberValidations, String weight_path) throws IOException{
		    
			this.problem = problem;
		    operador_crossover = _operador_crossover;
		    operador_mutacao = _operador_mutacao;
		    crossoverProbability = _crossoverProbability;
			mutationProbability = _mutationProbability;
			maxEvaluations = _numberValidations;
			this.weight_path = weight_path;
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
		    
		    algorithm = new MOMBI2(problem,maxEvaluations,crossover,mutation,selection,new SequentialSolutionListEvaluator<IntegerSolution>(),
		    		weight_path);
		    
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
