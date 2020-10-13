package algoritmos_evolucionarios;





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

import java.io.IOException;

import java.util.List;

import dependencias_abstract.AbstractAlgorithmRunner;
import dependencias_class.AlgorithmRunner;

import dependencias_class.BinaryTournamentSelection;
import dependencias_class.Saida;


/**
 * Class for configuring and running the IBEA algorithm
 *
 * @author Antonio J. Nebro <antonio@lcc.uma.es>
 */
public class IBEA_LLH_IntegerProblem extends AbstractAlgorithmRunner {
  
    private Problem<IntegerSolution> problem;
    private IBEA<IntegerSolution> algorithm;
    private CrossoverOperator<IntegerSolution> crossover;
    private MutationOperator<IntegerSolution> mutation;
    private SelectionOperator<List<IntegerSolution>, IntegerSolution> selection;
 
    

	private int populationSize = 0;
	private int operador_crossover = 0;
	private int operador_mutacao = 0;
	private Double crossoverProbability = 0.0;
	private Double mutationProbability = 0.0;
	private int maxEvaluations = 0; 
	private int numberArchievment = 0;

	public IBEA_LLH_IntegerProblem(Problem<IntegerSolution> problem, int _populationSize, Double _crossoverProbability, Double _mutationProbability, int _operador_crossover, int _operador_mutacao, int _numberValidations, int _numberArchievment) throws IOException{
	    

		
			this.problem = problem;

		    populationSize = _populationSize;
		    operador_crossover = _operador_crossover;
		    operador_mutacao = _operador_mutacao;
		    crossoverProbability = _crossoverProbability;
			mutationProbability = _mutationProbability;
			maxEvaluations = _numberValidations;
			numberArchievment = _numberArchievment;
			
	}
	
	
	public Saida execute() throws IOException {
	
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
	    
	    selection = new BinaryTournamentSelection<IntegerSolution>() ;
	
	    algorithm = new IBEABuilderIntegerProblem(problem)
	      .setArchiveSize(numberArchievment)
	      .setPopulationSize(populationSize)
	      .setMaxEvaluations(maxEvaluations)
	      .setCrossover(crossover)
	      .setMutation(mutation)
	      .setSelection(selection)
	      .build();
	
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

