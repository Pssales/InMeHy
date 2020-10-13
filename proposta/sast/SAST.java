package sast;

import static utilidades.SaveFiles.saveFunVar;

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
import utilidades.Impressora;
import utilidades.PFTrueKnown;
import utilidades.Populacoes;
import utilidades.SolucaoIndividual;
import utilidades.algoritmo;
import utilidades.auxiliar;

public class SAST {

	private static String cfgProp = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\config.properties"; // graphviz
	private static String TEMP_DIR = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\temp"; // graphviz

	private static int estudos_caso = 2;
	private static int maxTrials = 30;

	// arquivos
	private static String referenceParetoFront = "";
	private static String caminho_projeto = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\";

	private static int dimensao_problema = 3;
	private static int populationSize = 100;
	private static int adjustMombi = 12;
	private static int operador_crossover = 1;
	private static int operador_mutacao = 1;
	private static double crossoverProbability = 0.9;
	private static double mutationProbability = 0.0125;
	private static int numberValidations = 1100; //////// esse e o unico que muda;
	private static int numberArchievment = populationSize * numberValidations; //////// esse e o unico que muda;
	private static String weight_path = "C:\\Users\\Camila\\Dropbox\\metaheuristic\\geracao_weight_MOMBI\\weights\\output\\weight_03D_12.sld";

	public static void main(String[] args) throws IOException, InterruptedException {

		for (int ec = 2; ec <= estudos_caso; ec++) {

			Populacoes popsFinais = Populacoes.getInstance();
			PFTrueKnown pftrueknown = PFTrueKnown.getInstance();

			String str = "";
			System.out.println("#Mestrado - Integer Problem...");
			// modelo

			Problem<IntegerSolution> problem = new Camila_problema(caminho_projeto + "dots\\sast\\" + ec + ".dot");
			int contagem_solucao = 0;
			System.out.println("#############################");
			System.out.println("INICIANDO");

			SolucaoIndividual novo_MOMBI = new SolucaoIndividual();
			novo_MOMBI.setAlg(algoritmo.mombi);
			novo_MOMBI.setLlh("");
			novo_MOMBI.setTrial(maxTrials);
			for (int trial = 0; trial < maxTrials; trial++) {
				System.out.println("Algoritmo: MOMBI-II Contagem de execuï¿½ï¿½o: " + trial);
				MOMBI2_LLH_IntegerProblem llh = new MOMBI2_LLH_IntegerProblem(problem, crossoverProbability,
						mutationProbability, operador_crossover, operador_mutacao, numberValidations, weight_path);
				Saida popMOMBI = llh.execute();
				List<IntegerSolution> popMOMBIFinal = SolutionListUtils
						.getNondominatedSolutions(popMOMBI.getPopulacao_final());
				novo_MOMBI.addPop(popMOMBIFinal);
				pftrueknown.addPop(popMOMBI.getPopulacao_final());

				saveFunVar(trial, "MOMBI-II", problem, popMOMBI.getPopulacao_final(), ec);

				popMOMBI.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopMOMBITodoMundoMesmo(novo_MOMBI);

			System.out.println("#############################");

			SolucaoIndividual novo_IBEA = new SolucaoIndividual();
			novo_IBEA.setAlg(algoritmo.nsgaii);
			novo_IBEA.setLlh("");
			novo_IBEA.setTrial(maxTrials);
			for (int trial = 0; trial < maxTrials; trial++) {
				SolucaoIndividual novo = new SolucaoIndividual();
				System.out.println("Algoritmo: IBEA Contagem de execuï¿½ï¿½o: " + trial);
				IBEA_LLH_IntegerProblem llh = new IBEA_LLH_IntegerProblem(problem, populationSize, crossoverProbability,
						mutationProbability, operador_crossover, operador_mutacao, populationSize * numberValidations,
						numberArchievment);
				Saida popIBEA = llh.execute();
				List<IntegerSolution> popIBEAFinal = SolutionListUtils
						.getNondominatedSolutions(popIBEA.getPopulacao_final());
				novo_IBEA.addPop(popIBEAFinal);
				pftrueknown.addPop(popIBEA.getPopulacao_final());
				saveFunVar(trial, "IBEA", problem, popIBEA.getPopulacao_final(), ec);

				popIBEA.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopIBEATodoMundoMesmo(novo_IBEA);

			System.out.println("#############################");
			SolucaoIndividual novo_NSGAIII = new SolucaoIndividual();
			novo_NSGAIII.setAlg(algoritmo.nsgaiii);
			novo_NSGAIII.setLlh("");
			novo_NSGAIII.setTrial(maxTrials);
			for (int trial = 0; trial < maxTrials; trial++) {
				System.out.println("Algoritmo: NSGA-III Contagem de execuï¿½ï¿½o: " + trial);
				NSGAIII_LLH_IntegerProblem llh3 = new NSGAIII_LLH_IntegerProblem(problem, populationSize,
						crossoverProbability, mutationProbability, operador_crossover, operador_mutacao,
						numberValidations);
				Saida popNSGAIII = llh3.execute();
				List<IntegerSolution> popNSGAIIIFinal = SolutionListUtils
						.getNondominatedSolutions(popNSGAIII.getPopulacao_final());
				novo_NSGAIII.addPop(popNSGAIIIFinal);
				pftrueknown.addPop(popNSGAIII.getPopulacao_final());
				saveFunVar(trial, "NSGA-III", problem, popNSGAIII.getPopulacao_final(), ec);

				popNSGAIII.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopNSGAIIITodoMundoMesmo(novo_NSGAIII);

			System.out.println("#############################");
			SolucaoIndividual novo_SPEA2 = new SolucaoIndividual();
			novo_SPEA2.setAlg(algoritmo.spea2);
			novo_SPEA2.setLlh("");
			novo_SPEA2.setTrial(maxTrials);
			for (int trial = 0; trial < maxTrials; trial++) {
				System.out.println("Algoritmo: SPEA2 Contagem de execuï¿½ï¿½o: " + trial);
				SPEA2_LLH_IntegerProblem llh2 = new SPEA2_LLH_IntegerProblem(problem, populationSize,
						crossoverProbability, mutationProbability, operador_crossover, operador_mutacao,
						numberValidations);
				Saida popSPEA2 = llh2.execute();
				List<IntegerSolution> popSPEA2Final = SolutionListUtils
						.getNondominatedSolutions(popSPEA2.getPopulacao_final());
				novo_SPEA2.addPop(popSPEA2Final);
				pftrueknown.addPop(popSPEA2.getPopulacao_final());
				saveFunVar(trial, "SPEA-II", problem, popSPEA2.getPopulacao_final(), ec);
				popSPEA2.getPopulacao_final().clear();
				auxiliar.getInstance().limpa();
			}
			popsFinais.addAllpopSPEA2TodoMundoMesmo(novo_SPEA2);

			String pf = new String();
			for (IntegerSolution el : pftrueknown.getPftrueknow()) {
				if (!pf.contains(el.getObjective(0) + "\t" + el.getObjective(1) + "\t" + el.getObjective(2))) {
					pf = pf + el.getObjective(0) + "\t" + el.getObjective(1) + "\t" + el.getObjective(2) + "\n";
				}
			}

			Impressora.getInstance().imprimirArquivo(
					("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\SAST" + ec + "_3\\TrueKnownParetoFront.tsv"),
					pf);

			pftrueknown.gerarParetoFront();

			int l = 0;
			l = 0;

			for (SolucaoIndividual solucaoSPEA2_ : popsFinais.getAllpopSPEA2TodoMundoMesmo()) {

				solucaoSPEA2_.calcularIndicadores();
				String result_hyp_SPEA2 = "";
				String result_igd_SPEA2 = "";
				String result_eps_SPEA2 = "";

				for (String elemento : solucaoSPEA2_.getHypervolume()) {
					result_hyp_SPEA2 = result_hyp_SPEA2 + elemento + "\t";
				}
				for (String elemento : solucaoSPEA2_.getIgdPlus()) {
					result_igd_SPEA2 = result_igd_SPEA2 + elemento + "\t";
				}
				for (String elemento : solucaoSPEA2_.getEpsilon()) {
					result_eps_SPEA2 = result_eps_SPEA2 + elemento + "\t";
				}

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\SPEA-II\\SPEA-II_hypervolume.tsv"), result_hyp_SPEA2);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\SPEA-II\\SPEA-II_idgplus.tsv"), result_igd_SPEA2);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\SPEA-II\\SPEA-II_epsilon.tsv"), result_eps_SPEA2);

			}

			l = 0;
			for (SolucaoIndividual solucaoMOMBI2_ : popsFinais.getAllpopMOMBITodoMundoMesmo()) {

				solucaoMOMBI2_.calcularIndicadores();
				String result_hyp_MOMBI2 = "";
				String result_igd_MOMBI2 = "";
				String result_eps_MOMBI2 = "";

				for (String elemento : solucaoMOMBI2_.getHypervolume()) {
					result_hyp_MOMBI2 = result_hyp_MOMBI2 + elemento + "\t";
				}
				for (String elemento : solucaoMOMBI2_.getIgdPlus()) {
					result_igd_MOMBI2 = result_igd_MOMBI2 + elemento + "\t";
				}
				for (String elemento : solucaoMOMBI2_.getEpsilon()) {
					result_eps_MOMBI2 = result_eps_MOMBI2 + elemento + "\t";
				}

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\MOMBI-II\\MOMBI-II_hypervolume.tsv"), result_hyp_MOMBI2);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\MOMBI-II\\MOMBI-II_idgplus.tsv"), result_igd_MOMBI2);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\MOMBI-II\\MOMBI-II_epsilon.tsv"), result_eps_MOMBI2);

			}

			l = 0;
			for (SolucaoIndividual solucaoNSGAIII_ : popsFinais.getAllpopNSGAIIITodoMundoMesmo()) {

				solucaoNSGAIII_.calcularIndicadores();
				String result_hyp_NSGAIII = "";
				String result_igd_NSGAIII = "";
				String result_eps_NSGAIII = "";

				for (String elemento : solucaoNSGAIII_.getHypervolume()) {
					result_hyp_NSGAIII = result_hyp_NSGAIII + elemento + "\t";
				}
				for (String elemento : solucaoNSGAIII_.getIgdPlus()) {
					result_igd_NSGAIII = result_igd_NSGAIII + elemento + "\t";
				}
				for (String elemento : solucaoNSGAIII_.getEpsilon()) {
					result_eps_NSGAIII = result_eps_NSGAIII + elemento + "\t";
				}

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\NSGA-III\\NSGA-III_hypervolume.tsv"), result_hyp_NSGAIII);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\NSGA-III\\NSGA-III_idgplus.tsv"), result_igd_NSGAIII);

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\NSGA-III\\NSGA-III_epsilon.tsv"), result_eps_NSGAIII);

			}

			l = 0;
			for (SolucaoIndividual solucaoIBEA_ : popsFinais.getAllpopIBEATodoMundoMesmo()) {

				solucaoIBEA_.calcularIndicadores();
				String result_hyp_IBEA = "";
				String result_igd_IBEA = "";
				String result_eps_IBEA = "";

				for (String elemento : solucaoIBEA_.getHypervolume()) {
					result_hyp_IBEA = result_hyp_IBEA + elemento + "\t";
				}
				for (String elemento : solucaoIBEA_.getIgdPlus()) {
					result_igd_IBEA = result_igd_IBEA + elemento + "\t";
				}
				for (String elemento : solucaoIBEA_.getEpsilon()) {
					result_eps_IBEA = result_eps_IBEA + elemento + "\t";
				}

				Impressora.getInstance().imprimirArquivo(("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\TLTP" + ec
						+ "_2\\IBEA\\IBEA_hypervolume.tsv"), result_hyp_IBEA);

				Impressora.getInstance().imprimirArquivo(
						("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\SAST" + ec + "_3\\IBEA\\IBEA_idgplus.tsv"),
						result_igd_IBEA);

				Impressora.getInstance().imprimirArquivo(
						("C:\\Users\\Camila\\Dropbox\\metaheuristic\\files\\SAST" + ec + "_3\\IBEA\\IBEA_epsilon.tsv"),
						result_eps_IBEA);

			}

			System.out.println("Fim da execução");
			limparTudo();
			System.out.println("nova da execução");

		}
	}

	public static void limparTudo() {

		Populacoes.getInstance().getAllpopNSGAIIITodoMundoMesmo().clear();
		Populacoes.getInstance().getAllpopSPEA2TodoMundoMesmo().clear();
		Populacoes.getInstance().getAllpopMOMBITodoMundoMesmo().clear();
		Populacoes.getInstance().getAllpopIBEATodoMundoMesmo().clear();

		PFTrueKnown.getInstance().getPftrueknow().clear();

	}

}
