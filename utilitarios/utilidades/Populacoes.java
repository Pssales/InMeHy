package utilidades;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import dependencias_interfaces.IntegerSolution;

public class Populacoes {
	
	
	private List<SolucaoIndividual> allpopIBEATodoMundoMesmo; 	
	private List<SolucaoIndividual> allpopMOMBITodoMundoMesmo;
	private List<SolucaoIndividual> allpopNSGAIITodoMundoMesmo;
	private List<SolucaoIndividual> allpopNSGAIIITodoMundoMesmo;
	private List<SolucaoIndividual> allpopSPEA2TodoMundoMesmo;
	private List<SolucaoIndividual> allpopHUGSTodoMundoMesmo;
	private List<SolucaoIndividual> allpopGEMOITOTodoMundoMesmo;
	private List<SolucaoIndividual> allpopGEMOITOTodoMundoMesmo_HLH;
	private List<IntegerSolution> popHUGS;
	private String algoritmo;


	private static Populacoes uniqueInstance;
	
	private Populacoes() {
		allpopIBEATodoMundoMesmo = new ArrayList<SolucaoIndividual>();		
		allpopMOMBITodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopNSGAIITodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopNSGAIIITodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopSPEA2TodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopHUGSTodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopGEMOITOTodoMundoMesmo = new ArrayList<SolucaoIndividual>();
		allpopGEMOITOTodoMundoMesmo_HLH = new ArrayList<SolucaoIndividual>();
		popHUGS = new ArrayList<IntegerSolution>();
		algoritmo = "";
	}
	
	
	public static Populacoes getInstance() {
		if(uniqueInstance==null) {
			uniqueInstance = new Populacoes();
		}
		return uniqueInstance;
	}

	public void addAllpopIBEATodoMundoMesmo(SolucaoIndividual popIBEA) {
		SolucaoIndividual novo_1 = new SolucaoIndividual();
		novo_1 = popIBEA.clonar();
		allpopIBEATodoMundoMesmo.add(novo_1);
	}
	
	public void addAllpopMOMBITodoMundoMesmo(SolucaoIndividual popMOMBI) {
		SolucaoIndividual novo_1 = new SolucaoIndividual();
		novo_1 = popMOMBI.clonar();
		allpopMOMBITodoMundoMesmo.add(novo_1);
	}
	public void addAllpopNSGAIITodoMundoMesmo(SolucaoIndividual popNSGAII) {
		SolucaoIndividual novo_1 = new SolucaoIndividual();
		novo_1 = popNSGAII.clonar();
		allpopNSGAIITodoMundoMesmo.add(novo_1);
	}
	public void addAllpopNSGAIIITodoMundoMesmo(SolucaoIndividual popNSGAIII) {
		SolucaoIndividual novo_1 = new SolucaoIndividual();
		novo_1 = popNSGAIII.clonar();
		allpopNSGAIIITodoMundoMesmo.add(novo_1);
	}
	public void addAllpopSPEA2TodoMundoMesmo(SolucaoIndividual popSPEA2) {
		SolucaoIndividual novo_1 = new SolucaoIndividual();
		novo_1 = popSPEA2.clonar();
		allpopSPEA2TodoMundoMesmo.add(novo_1);
	}


	public List<SolucaoIndividual> getAllpopIBEATodoMundoMesmo() {
		return allpopIBEATodoMundoMesmo;
	}


	public void setAllpopIBEATodoMundoMesmo(List<SolucaoIndividual> allpopIBEATodoMundoMesmo) {
		this.allpopIBEATodoMundoMesmo = allpopIBEATodoMundoMesmo;
	}


	public List<SolucaoIndividual> getAllpopMOMBITodoMundoMesmo() {
		return allpopMOMBITodoMundoMesmo;
	}


	public void setAllpopMOMBITodoMundoMesmo(List<SolucaoIndividual> allpopMOMBITodoMundoMesmo) {
		this.allpopMOMBITodoMundoMesmo = allpopMOMBITodoMundoMesmo;
	}


	public List<SolucaoIndividual> getAllpopNSGAIIITodoMundoMesmo() {
		return allpopNSGAIIITodoMundoMesmo;
	}


	public void setAllpopNSGAIIITodoMundoMesmo(List<SolucaoIndividual> allpopNSGAIIITodoMundoMesmo) {
		this.allpopNSGAIIITodoMundoMesmo = allpopNSGAIIITodoMundoMesmo;
	}


	public List<SolucaoIndividual> getAllpopSPEA2TodoMundoMesmo() {
		return allpopSPEA2TodoMundoMesmo;
	}

	public void setAllpopSPEA2TodoMundoMesmo(List<SolucaoIndividual> allpopSPEA2TodoMundoMesmo) {
		this.allpopSPEA2TodoMundoMesmo = allpopSPEA2TodoMundoMesmo;
	}


	public String getAlgoritmo() {
		return algoritmo;
	}


	public void setAlgoritmo(String algoritmo) {
		this.algoritmo = algoritmo;
	}


	
}
