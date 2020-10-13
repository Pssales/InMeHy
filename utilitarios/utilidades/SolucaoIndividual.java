package utilidades;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import dependencias_class.ArrayFront;
import dependencias_class.Epsilon;
import dependencias_class.FrontUtils;
import dependencias_class.InvertedGenerationalDistancePlus;
import dependencias_class.PISAHypervolume;
import dependencias_class.PointSolution;
import dependencias_interfaces.Front;
import dependencias_interfaces.IntegerSolution;
import dicionarios.ConjuntoTestes;

public class SolucaoIndividual {


	private algoritmo alg;
	private int trial;
	private String llh;
	private List<String> hypervolume;
	private List<String> igdPlus;
	private List<String> epsilon;
	private List<List<IntegerSolution>> populacao;


	public SolucaoIndividual() {
		populacao = new ArrayList<List<IntegerSolution>>();
		hypervolume = new ArrayList<String>();
		igdPlus = new ArrayList<String>();
		epsilon = new ArrayList<String>();

	}

	
	public void addPop(List<IntegerSolution> pop) {
		populacao.add(pop);
	}
	public algoritmo getAlg() {
		return alg;
	}
	public void setAlg(algoritmo alg) {
		this.alg = alg;
	}
	public int getTrial() {
		return trial;
	}
	public void setTrial(int trial) {
		this.trial = trial;
	}

	public String getLlh() {
		return llh;
	}

	public void setLlh(String llh) {
		this.llh = llh;
	}

	public List<List<IntegerSolution>> getPopulacao() {
		return populacao;
	}

	public void setPopulacao(List<List<IntegerSolution>> populacao) {
		this.populacao = populacao;
	}

	public List<String> getHypervolume() {
		return hypervolume;
	}

	public void setHypervolume(List<String> hypervolume) {
		this.hypervolume = hypervolume;
	}

	public List<String> getIgdPlus() {
		return igdPlus;
	}

	public void setIgdPlus(List<String> igdPlus) {
		this.igdPlus = igdPlus;
	}

	public List<String> getEpsilon() {
		return epsilon;
	}

	public void setEpsilon(List<String> epsilon) {
		this.epsilon = epsilon;
	}

	public SolucaoIndividual clonar() {

		
		
		SolucaoIndividual novo = new SolucaoIndividual();

		ArrayList<List<IntegerSolution>> clone_conj_pop = new ArrayList<List<IntegerSolution>>();
		ArrayList<IntegerSolution> clone_pop = new ArrayList<IntegerSolution>();

		for(List<IntegerSolution> pop: populacao) {
			for(IntegerSolution i : pop) {
				clone_pop.add((IntegerSolution) i.copy());	
			}
			clone_conj_pop.add((List<IntegerSolution>) clone_pop.clone());
			clone_pop.clear();
		}

		novo.setPopulacao(clone_conj_pop);
		novo.setAlg(this.alg);
		
		ArrayList<String> novo_h = new ArrayList<String>();
		for(String h : hypervolume) {
			novo_h.add(h);
		}
		novo.setHypervolume(novo_h);
		ArrayList<String> novo_e = new ArrayList<String>();
		for(String e : epsilon) {
			novo_e.add(e);
		}
		novo.setEpsilon(novo_e);
		ArrayList<String> novo_i = new ArrayList<String>();
		for(String i : igdPlus) {
			novo_i.add(i);
		}
		novo.setIgdPlus(novo_i);
		novo.setLlh(this.llh);
		novo.setTrial(this.trial);
		
		
		return novo;
	}

	public void calcularIndicadores() {

	//	System.out.println("Pop: "+populacao.size());
		
		for(List<IntegerSolution> popFinal_ : populacao) {
			
			Front normalizedFront = PFTrueKnown.getInstance().getFrontNormalizer().normalize(new ArrayFront(popFinal_)) ;
			List<PointSolution> normalizedPopulation = FrontUtils
					.convertFrontToSolutionList(normalizedFront) ;
			
			String result_hyp = new PISAHypervolume<PointSolution>(PFTrueKnown.getInstance().getNormalizedReferenceFront()).evaluate(normalizedPopulation) + "";
//			String result_eps = new Epsilon<PointSolution>(PFTrueKnown.getInstance().getNormalizedReferenceFront()).evaluatee(popFinal_) + " ";
			String result_eps = new Epsilon<PointSolution>(PFTrueKnown.getInstance().getNormalizedReferenceFront()).evaluate(normalizedPopulation) + " ";
			String result_igd = new InvertedGenerationalDistancePlus<PointSolution>(PFTrueKnown.getInstance().getNormalizedReferenceFront()).evaluate(normalizedPopulation) + "";

			hypervolume.add(result_hyp);
			igdPlus.add(result_igd);
			epsilon.add(result_eps);
			
			result_hyp = "";
			result_eps = "";
			result_igd = "";
			
			normalizedPopulation.clear();
		}
	}
	
	
	public double getMediaHypervolume() {
		double media_hipervolume = 0.0;
		for(String e : hypervolume) {
			media_hipervolume = media_hipervolume + Double.parseDouble(e);
		}
		return (media_hipervolume/hypervolume.size());
	}
	
	public double getMediaIGDPlus() {
		double media_idgplus = 0.0;
		for(String e : igdPlus) {
			media_idgplus = media_idgplus + Double.parseDouble(e);
		}
		return (media_idgplus/igdPlus.size());
	}
	
	public void buscarMelhorSolucao() {
		

		double menor = populacao.get(0).get(0).getObjective(0);
		IntegerSolution ma = populacao.get(0).get(0);
		for(List<IntegerSolution> popFinal_ : populacao) {
			for(IntegerSolution elemento : popFinal_) {
				if(elemento.getObjective(0)<menor) {
					menor = elemento.getObjective(0);
					ma = elemento;
				}
			}
		}
		System.out.println(ma);
		
	}
	
}
