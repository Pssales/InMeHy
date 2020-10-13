package utilidades;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

import com.opencsv.CSVReader;




// como funciona ?

// caminho 1 serve para ...
// caminho 2 serve para ...

public class gerarTabelasLatex {

	private static String caminho_1 = "/home/juliana/eclipse-workspace/HUGS/files/resultados_nativos.csv";
	private static String caminho_2 = "/home/juliana/eclipse-workspace/HUGS/files/resultados_HUGS.csv";

	public static void main(String[] args) throws IOException {

		String[] algs = {"NSGA-II", "MOMBI", "IBEA", "NSGA-III", "HUGS_0"};
		int indice = 4;
		int estudos_caso = 15;
		
		
		
		System.out.println("\\begin{table}[!ht]" 
				+ "\n" + "\\begin{center}"); 

		if(indice==2) {
			System.out.println("\\caption{Hypervolume averages}");	
		}
		if(indice==3) {
			System.out.println("\\caption{Epsilon averages}");	
		}
		if(indice==4) {
			System.out.println("\\caption{IGD+ averages}");	
		}


		System.out.println("\\resizebox{1\\textwidth}{!}{\\begin{minipage}{2.6\\columnwidth}"
				+ "\n" +  "\\begin{tabular}{c c c c c c}" 
				+ "\n" +  "\\toprule" 
				+ "\n" +  "{\\footnotesize \\textbf{\\textit{Case Study}}}&{\\footnotesize \\textbf{\\textit{NSGA-II}}} & "
				+ "\n" +  "{\\footnotesize \\textbf{\\textit{MOMBI}}} & "
				+ "\n" +  "{\\footnotesize \\textbf{\\textit{IBEA}}} & "
				+ "\n" +  "{\\footnotesize \\textbf{\\textit{NSGA-III}}} &"
				+ "\n" +  "{\\footnotesize \\textbf{\\textit{HUGS}}}  \\\\"
				+ "\n" + "\\midrule");




		// hipervolume 2
		// epsilon 3
		// igd+ 4

		for(int i=1; i<=estudos_caso; i++){
			String linha_tabela = "";
			for(String alg : algs){
				String retorno = (buscaRegistro("\""+i+"\"", "\""+alg+"\"", indice));
				linha_tabela = linha_tabela + " & ";


				if(indice==2) {
					// hypervolume
					if(retorno.length()>3){
						linha_tabela = linha_tabela + retorno.substring(0,7).replace("\"", "") + "E-1";
					} else{
						linha_tabela = linha_tabela + retorno.replace("\"", "") + "E-1";
					}
				}


				if(indice==3) {
					// epsilon
					if(retorno.length()>3){
						linha_tabela = linha_tabela + retorno.substring(0,4).replace("\"", "");
					} else{
						linha_tabela = linha_tabela + retorno.replace("\"", "");
					}
				}

				if(indice==4) {
					// IGD+
					if(retorno.length()>3){
						linha_tabela = linha_tabela + retorno.substring(0,5).replace("\"", "");
					} else{
						linha_tabela = linha_tabela + retorno.replace("\"", "");
					}
				}
			}
			System.out.println(i + linha_tabela + " \\\\");
		}
		System.out.println("\\bottomrule "
				+ "\n" + "\\end{tabular} "
				+ "\n" + "\\end{minipage}} "
				+ "\n" + "\\label{int1} "
				+ "\n" + "\\end{center} "
				+ "\n" + "\\end{table}");
	}	

	public static String buscaRegistro(String estudo_caso, String alg, int indicador){

		String retorno = "";

		try {

			FileReader arq = new FileReader(caminho_1);
			BufferedReader lerArq = new BufferedReader(arq);

			String linha = lerArq.readLine(); 

			while (linha != null) {

				int cont = 0;
				boolean ver_estudo_caso = false;
				boolean ver_alg = false;


				for(String elemento : linha.split("\t")){	

					if(cont==0 && elemento.equals(estudo_caso)){
						ver_estudo_caso = true;
						cont = cont + 1;
						continue;
					}

					if(cont==1 && elemento.equals(alg) && ver_estudo_caso==true){
						ver_alg = true;
						cont = cont + 1;
						continue;
					}

					if(cont==indicador && ver_alg==true && ver_estudo_caso==true){
						retorno = elemento;
						break;
					}
					cont = cont + 1;
				}

				linha = lerArq.readLine();
			}
			arq.close();

		} catch (IOException e) {
			System.err.printf("Erro na abertura do arquivo: %s.\n",
					e.getMessage());
		}


		try {

			FileReader arq = new FileReader(caminho_2);
			BufferedReader lerArq = new BufferedReader(arq);

			String linha = lerArq.readLine(); 

			while (linha != null) {

				int cont = 0;
				boolean ver_estudo_caso = false;
				boolean ver_alg = false;


				for(String elemento : linha.split("\t")){	

					if(cont==0 && elemento.equals(estudo_caso)){
						ver_estudo_caso = true;
						cont = cont + 1;
						continue;
					}

					if(cont==1 && elemento.equals(alg) && ver_estudo_caso==true){
						ver_alg = true;
						cont = cont + 1;
						continue;
					}

					if(cont==indicador && ver_alg==true && ver_estudo_caso==true){
						retorno = elemento;
						break;
					}



					cont = cont + 1;

				}

				linha = lerArq.readLine();
			}
			arq.close();

		} catch (IOException e) {
			System.err.printf("Erro na abertura do arquivo: %s.\n",
					e.getMessage());
		}

		return retorno;

	}

}








