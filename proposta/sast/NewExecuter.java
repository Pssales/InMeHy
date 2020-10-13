package sast;

import java.io.IOException;
import java.io.Writer;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;

import com.opencsv.CSVWriter;

import algoritmos_evolucionarios.IBEA_LLH_IntegerProblem;
import algoritmos_evolucionarios.NSGAIII_LLH_IntegerProblem;
import algoritmos_evolucionarios.SPEA2_LLH_IntegerProblem;
import algoritmos_evolucionarios.mombi.MOMBI2_LLH_IntegerProblem;
import dependencias_class.Saida;
import dependencias_class.SolutionListUtils;
import dependencias_interfaces.IntegerSolution;
import dependencias_interfaces.Problem;
import problemas.Camila_problema;
import utilidades.PFTrueKnown;
import utilidades.Populacoes;
import utilidades.SolucaoIndividual;
import utilidades.algoritmo;
import utilidades.auxiliar;


public class NewExecuter {

	private static String cfgProp = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\config.properties"; // graphviz
	private static String TEMP_DIR = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\temp"; //  graphviz

	private static int estudos_caso = 3;
	private static int maxTrials = 30;

	//arquivos
	private static String referenceParetoFront = "";
	private static String caminho_projeto = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\";

	private static int dimensao_problema = 3;
	private static int populationSize = 100;
	private static int adjustMombi = 12;
	private static int operador_crossover = 1;
	private static int operador_mutacao = 1;
	private static double crossoverProbability = 0.9;
	private static double mutationProbability = 0.00125;
	private static int numberValidations = 1000;  //////// esse e o unico que muda;
	private static int numberArchievment = populationSize*numberValidations;  //////// esse e o unico que muda;
	private static String weight_path = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\geracao_weight_MOMBI\\weights\\output\\weight_03D_12.sld";
	public static void main(String[] args) throws IOException, InterruptedException {

		for(int ec=3; ec<=estudos_caso; ec++){

			Populacoes popsFinais = Populacoes.getInstance();
			PFTrueKnown pftrueknown = PFTrueKnown.getInstance();

			String str = "";
			System.out.println("Algumas dicas: se der errado com o mombi, apague os arquivos weight_path e crie novamente uma unica vez !!!!!!");
			System.out.println("#Mestrado - Integer Problem...");
			//modelo 

			Problem<IntegerSolution> problem = new Camila_problema(caminho_projeto+"dots\\"+ec+".dot");
			int contagem_solucao = 0;
			System.out.println("#############################");
			System.out.println("INICIANDO NATIVOS");
			
			SolucaoIndividual novo_IBEA = new SolucaoIndividual();
			novo_IBEA.setAlg(algoritmo.nsgaii);
			novo_IBEA.setLlh("");
			novo_IBEA.setTrial(maxTrials);
			for(int trial=0; trial<maxTrials; trial++) {
				SolucaoIndividual novo = new SolucaoIndividual();
				System.out.println("Algoritmo: IBEA Contagem de execu��o: "+trial);
				IBEA_LLH_IntegerProblem llh = new IBEA_LLH_IntegerProblem(problem, populationSize, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao,  populationSize*numberValidations, numberArchievment);
				Saida popIBEA = llh.execute();
				List<IntegerSolution> popIBEAFinal = SolutionListUtils.getNondominatedSolutions(popIBEA.getPopulacao_final());
				novo_IBEA.addPop(popIBEAFinal);					
				pftrueknown.addPop(popIBEA.getPopulacao_final()); 
				popIBEA.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopIBEATodoMundoMesmo(novo_IBEA);

			
			System.out.println("#############################");
			SolucaoIndividual novo_MOMBI = new SolucaoIndividual();
			novo_MOMBI.setAlg(algoritmo.mombi);
			novo_MOMBI.setLlh("");
			novo_MOMBI.setTrial(maxTrials);
			for(int trial=0; trial<maxTrials; trial++) {
				System.out.println("Algoritmo: MOMBI Contagem de execu��o: "+trial);
				MOMBI2_LLH_IntegerProblem llh = new MOMBI2_LLH_IntegerProblem(problem, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations, weight_path);
				Saida popMOMBI = llh.execute();
				List<IntegerSolution> popMOMBIFinal = SolutionListUtils.getNondominatedSolutions(popMOMBI.getPopulacao_final());
				novo_MOMBI.addPop(popMOMBIFinal);					
				pftrueknown.addPop(popMOMBI.getPopulacao_final()); 
				popMOMBI.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopMOMBITodoMundoMesmo(novo_MOMBI);
			
			System.out.println("#############################");
			SolucaoIndividual novo_NSGAIII = new SolucaoIndividual();
			novo_NSGAIII.setAlg(algoritmo.nsgaiii);
			novo_NSGAIII.setLlh("");
			novo_NSGAIII.setTrial(maxTrials);
			for(int trial=0; trial<maxTrials; trial++) {
				System.out.println("Algoritmo: NSGA-III Contagem de execu��o: "+trial);
				NSGAIII_LLH_IntegerProblem llh3 = new NSGAIII_LLH_IntegerProblem(problem, populationSize, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations);
				Saida popNSGAIII = llh3.execute();
				List<IntegerSolution> popNSGAIIIFinal= SolutionListUtils.getNondominatedSolutions(popNSGAIII.getPopulacao_final());
				novo_NSGAIII.addPop(popNSGAIIIFinal);					
				pftrueknown.addPop(popNSGAIII.getPopulacao_final()); 
				popNSGAIII.getPopulacao_final().clear();	
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopNSGAIIITodoMundoMesmo(novo_NSGAIII);
			
			System.out.println("#############################");
			SolucaoIndividual novo_SPEA2 = new SolucaoIndividual();
			novo_SPEA2.setAlg(algoritmo.spea2);
			novo_SPEA2.setLlh("");
			novo_SPEA2.setTrial(maxTrials);
			for(int trial=0; trial<maxTrials; trial++) {
				System.out.println("Algoritmo: SPEA2 Contagem de execu��o: "+contagem_solucao);
				SPEA2_LLH_IntegerProblem llh2 = new SPEA2_LLH_IntegerProblem(problem, populationSize, crossoverProbability, mutationProbability, operador_crossover, operador_mutacao, numberValidations);
				Saida popSPEA2 = llh2.execute();
				List<IntegerSolution> popSPEA2Final= SolutionListUtils.getNondominatedSolutions(popSPEA2.getPopulacao_final());
				novo_SPEA2.addPop(popSPEA2Final);					
				pftrueknown.addPop(popSPEA2.getPopulacao_final()); 
				popSPEA2.getPopulacao_final().clear();	
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopSPEA2TodoMundoMesmo(novo_MOMBI);
			
			String pf = new String();
			for(IntegerSolution el : pftrueknown.getPftrueknow()) {
				pf = pf + el.getObjective(0) + " " + el.getObjective(1) + " " + el.getObjective(2) + "\n";
			}

			pftrueknown.gerarParetoFront();
			
			int l = 0;			
			l=0;
			System.out.println("---------------------------");

			System.out.println("popsFinais " + popsFinais.getAllpopSPEA2TodoMundoMesmo().size());
			System.out.println("---------------------------");

			for(SolucaoIndividual solucaoSPEA2_ : popsFinais.getAllpopSPEA2TodoMundoMesmo()){		
			
				solucaoSPEA2_.calcularIndicadores();
				String result_hyp_SPEA2 = "";
				String result_igd_SPEA2 = "";
				String result_eps_SPEA2 = "";
				System.out.println("---------------------------");
				System.out.println("Solu��o "+solucaoSPEA2_.getHypervolume().size());
				System.out.println("---------------------------");

				for(String elemento : solucaoSPEA2_.getHypervolume()) {
					result_hyp_SPEA2 = result_hyp_SPEA2 + elemento + "\t";
				}
				for(String elemento : solucaoSPEA2_.getIgdPlus()) {
					result_igd_SPEA2 = result_igd_SPEA2 + elemento + "\t";
				}
				for(String elemento : solucaoSPEA2_.getIgdPlus()) {
					result_eps_SPEA2 = result_eps_SPEA2 + elemento + "\t";
				}
				
				List<String[]> linhas_hyp_SPEA2 = new ArrayList<>();
				linhas_hyp_SPEA2.add(new String[]{"SPEA2", result_hyp_SPEA2});
				Writer writer_hyp_SPEA2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_hyp_SPEA2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_hyp_SPEA2 = new CSVWriter(writer_hyp_SPEA2, '\t'); 
				csvWriter_hyp_SPEA2.writeAll(linhas_hyp_SPEA2);
				csvWriter_hyp_SPEA2.flush();	
				
				List<String[]> linhas_eps_SPEA2 = new ArrayList<>();
				linhas_eps_SPEA2.add(new String[]{"SPEA2", result_eps_SPEA2});
				Writer writer_eps_SPEA2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_eps_SPEA2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_eps_SPEA2 = new CSVWriter(writer_eps_SPEA2, '\t'); 
				csvWriter_eps_SPEA2.writeAll(linhas_eps_SPEA2);
				csvWriter_eps_SPEA2.flush();	
			
				List<String[]> linhas_igd_SPEA2 = new ArrayList<>();
				linhas_igd_SPEA2.add(new String[]{"SPEA2", result_igd_SPEA2});
				Writer writer_igd_SPEA2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_igd_SPEA2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_igd_SPEA2 = new CSVWriter(writer_igd_SPEA2, '\t'); 
				csvWriter_igd_SPEA2.writeAll(linhas_igd_SPEA2);
				csvWriter_igd_SPEA2.flush();	
				
			}
			
			l=0;
			for(SolucaoIndividual solucaoMOMBI2_ : popsFinais.getAllpopMOMBITodoMundoMesmo()){		
			
				solucaoMOMBI2_.calcularIndicadores();
				String result_hyp_MOMBI2 = "";
				String result_igd_MOMBI2 = "";
				String result_eps_MOMBI2 = "";
				
				for(String elemento : solucaoMOMBI2_.getHypervolume()) {
					result_hyp_MOMBI2 = result_hyp_MOMBI2 + elemento + "\t";
				}
				for(String elemento : solucaoMOMBI2_.getIgdPlus()) {
					result_igd_MOMBI2 = result_igd_MOMBI2 + elemento + "\t";
				}
				for(String elemento : solucaoMOMBI2_.getIgdPlus()) {
					result_eps_MOMBI2 = result_eps_MOMBI2 + elemento + "\t";
				}
				
								
				List<String[]> linhas_hyp_MOMBI2 = new ArrayList<>();
				linhas_hyp_MOMBI2.add(new String[]{"MOMBI2", result_hyp_MOMBI2});
				Writer writer_hyp_MOMBI2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_hyp_MOMBI2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_hyp_MOMBI2 = new CSVWriter(writer_hyp_MOMBI2, '\t'); 
				csvWriter_hyp_MOMBI2.writeAll(linhas_hyp_MOMBI2);
				csvWriter_hyp_MOMBI2.flush();	
				
				List<String[]> linhas_eps_MOMBI2 = new ArrayList<>();
				linhas_eps_MOMBI2.add(new String[]{"MOMBI2", result_eps_MOMBI2});
				Writer writer_eps_MOMBI2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_eps_MOMBI2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_eps_MOMBI2 = new CSVWriter(writer_eps_MOMBI2, '\t'); 
				csvWriter_eps_MOMBI2.writeAll(linhas_eps_MOMBI2);
				csvWriter_eps_MOMBI2.flush();	
			
				List<String[]> linhas_igd_MOMBI2 = new ArrayList<>();
				linhas_igd_MOMBI2.add(new String[]{"MOMBI2", result_igd_MOMBI2});
				Writer writer_igd_MOMBI2 = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_igd_MOMBI2.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_igd_MOMBI2 = new CSVWriter(writer_igd_MOMBI2, '\t'); 
				csvWriter_igd_MOMBI2.writeAll(linhas_igd_MOMBI2);
				csvWriter_igd_MOMBI2.flush();	
				
			}
			
			
			l=0;
			for(SolucaoIndividual solucaoNSGAIII_ : popsFinais.getAllpopNSGAIIITodoMundoMesmo()){		
			
				solucaoNSGAIII_.calcularIndicadores();
				String result_hyp_NSGAIII = "";
				String result_igd_NSGAIII = "";
				String result_eps_NSGAIII = "";
				
				for(String elemento : solucaoNSGAIII_.getHypervolume()) {
					result_hyp_NSGAIII = result_hyp_NSGAIII + elemento + "\t";
				}
				for(String elemento : solucaoNSGAIII_.getIgdPlus()) {
					result_igd_NSGAIII = result_igd_NSGAIII + elemento + "\t";
				}
				for(String elemento : solucaoNSGAIII_.getIgdPlus()) {
					result_eps_NSGAIII = result_eps_NSGAIII + elemento + "\t";
				}
				
				List<String[]> linhas_hyp_NSGAIII = new ArrayList<>();
				linhas_hyp_NSGAIII.add(new String[]{"NSGAIII", result_hyp_NSGAIII});
				Writer writer_hyp_NSGAIII = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_hyp_NSGAIII.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_hyp_NSGAIII = new CSVWriter(writer_hyp_NSGAIII, '\t'); 
				csvWriter_hyp_NSGAIII.writeAll(linhas_hyp_NSGAIII);
				csvWriter_hyp_NSGAIII.flush();	
				
				List<String[]> linhas_eps_NSGAIII = new ArrayList<>();
				linhas_eps_NSGAIII.add(new String[]{"NSGAIII", result_eps_NSGAIII});
				Writer writer_eps_NSGAIII = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_eps_NSGAIII.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_eps_NSGAIII = new CSVWriter(writer_eps_NSGAIII, '\t'); 
				csvWriter_eps_NSGAIII.writeAll(linhas_eps_NSGAIII);
				csvWriter_eps_NSGAIII.flush();	
			
				List<String[]> linhas_igd_NSGAIII = new ArrayList<>();
				linhas_igd_NSGAIII.add(new String[]{"NSGAIII", result_igd_NSGAIII});
				Writer writer_igd_NSGAIII = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_igd_NSGAIII.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_igd_NSGAIII = new CSVWriter(writer_igd_NSGAIII, '\t'); 
				csvWriter_igd_NSGAIII.writeAll(linhas_igd_NSGAIII);
				csvWriter_igd_NSGAIII.flush();	
				
			}
			
			
			l=0;
			for(SolucaoIndividual solucaoIBEA_ : popsFinais.getAllpopIBEATodoMundoMesmo()){		
			
				solucaoIBEA_.calcularIndicadores();
				String result_hyp_IBEA = "";
				String result_igd_IBEA = "";
				String result_eps_IBEA = "";
				
				for(String elemento : solucaoIBEA_.getHypervolume()) {
					result_hyp_IBEA = result_hyp_IBEA + elemento + "\t";
				}
				for(String elemento : solucaoIBEA_.getIgdPlus()) {
					result_igd_IBEA = result_igd_IBEA + elemento + "\t";
				}
				for(String elemento : solucaoIBEA_.getIgdPlus()) {
					result_eps_IBEA = result_eps_IBEA + elemento + "\t";
				}
				
				List<String[]> linhas_hyp_IBEA = new ArrayList<>();
				linhas_hyp_IBEA.add(new String[]{"IBEA", result_hyp_IBEA});
				Writer writer_hyp_IBEA = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_hyp_IBEA.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_hyp_IBEA = new CSVWriter(writer_hyp_IBEA, '\t'); 
				csvWriter_hyp_IBEA.writeAll(linhas_hyp_IBEA);
				csvWriter_hyp_IBEA.flush();	
				
				List<String[]> linhas_eps_IBEA = new ArrayList<>();
				linhas_eps_IBEA.add(new String[]{"IBEA", result_eps_IBEA});
				Writer writer_eps_IBEA = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_eps_IBEA.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_eps_IBEA = new CSVWriter(writer_eps_IBEA, '\t'); 
				csvWriter_eps_IBEA.writeAll(linhas_eps_IBEA);
				csvWriter_eps_IBEA.flush();	
			
				List<String[]> linhas_igd_IBEA = new ArrayList<>();
				linhas_igd_IBEA.add(new String[]{"IBEA", result_igd_IBEA});
				Writer writer_igd_IBEA = Files.newBufferedWriter(Paths.get("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\resultados_igd_IBEA.csv"), StandardOpenOption.APPEND);
				CSVWriter csvWriter_igd_IBEA = new CSVWriter(writer_igd_IBEA, '\t'); 
				csvWriter_igd_IBEA.writeAll(linhas_igd_IBEA);
				csvWriter_igd_IBEA.flush();	
				
			}
					
			System.out.println("Fim do algoritmo!");


		}
	}

}
