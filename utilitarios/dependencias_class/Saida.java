package dependencias_class;

import java.util.List;

import dependencias_interfaces.IntegerSolution;

public class Saida {

	private List<IntegerSolution> populacao_final;
	private Double objetivo_1;
	private Double objetivo_2;
	
	
	public Saida(List<IntegerSolution> a, Double b, Double c){
		populacao_final = a;
		objetivo_1 = b;
		objetivo_2 = c;
	}


	public List<IntegerSolution> getPopulacao_final() {
		return populacao_final;
	}


	public void setPopulacao_final(List<IntegerSolution> populacao_final) {
		this.populacao_final = populacao_final;
	}


	public Double getObjetivo_1() {
		return objetivo_1;
	}


	public void setObjetivo_1(Double objetivo_1) {
		this.objetivo_1 = objetivo_1;
	}


	public Double getObjetivo_2() {
		return objetivo_2;
	}


	public void setObjetivo_2(Double objetivo_2) {
		this.objetivo_2 = objetivo_2;
	}

}
