package problemas;

import java.util.LinkedList;

public class Dicionario {
	
	private LinkedList<Elemento_dicionario> dicionario;
	
	
	public Dicionario() {
		dicionario = new LinkedList<Elemento_dicionario>();
	}


	public LinkedList<Elemento_dicionario> getDicionario() {
		return dicionario;
	}


	public void setDicionario(LinkedList<Elemento_dicionario> dicionario) {
		this.dicionario = dicionario;
	}
	
	
	public String buscar_a_partir_da_chave(int chave) {
		for(Elemento_dicionario elemento : dicionario) {
			if(elemento.getChave()==chave) {
				return elemento.getValor();
			}
		}
		System.out.println("null");
		return "null";
	}
	
	public int buscar_a_partir_do_valor(String valor) {
		for(Elemento_dicionario elemento : dicionario) {
			if(elemento.getValor().equals(valor)) {
				return elemento.getChave();
			}
		}
		System.out.println("-1");
		return -1;
	}
	
	
	public void addElemento(Elemento_dicionario elemento) {
		dicionario.add(elemento);
	}
	
	
	
}
