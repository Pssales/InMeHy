package utilidades;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import utilidades.Impressora;



public class Impressora {
	
	private static Impressora uniqueInstance;
		
		public static Impressora getInstance(){
			if(uniqueInstance==null){
				uniqueInstance = new Impressora();
			}
			return uniqueInstance;
		}
		
		private Impressora(){}
	
	public void imprimirArquivo(String nome_arquivo, String frase) throws IOException{
			
			FileWriter saida = new FileWriter(nome_arquivo, true);
			PrintWriter gravarArquivo = new PrintWriter(saida);
			gravarArquivo.print(frase);
			saida.close();
			gravarArquivo.close();
		}
 
}
