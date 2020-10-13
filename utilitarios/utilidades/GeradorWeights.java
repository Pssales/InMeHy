package utilidades;


import java.io.IOException;

import dependencias_class.WeightVectors;



public class GeradorWeights {
	
	
	public static void main(String args[]) throws IOException{
		
		
		int populationSize = 130;
		int dimension = 3;
		double epsilon = 1.0;
		String caminho = "C:\\Users\\camil\\eclipse-workspace\\jMetal\\resources\\weightVectorFiles\\mombi2\\weight_03D_"+populationSize+"_mombi.sld";
		
		Impressora.getInstance().imprimirArquivo(caminho,"# "+populationSize+" "+dimension+"\n");
		
		System.out.println("GERADOR DE VETOR DE PESOS PARA O MOMBI");
		System.out.println("populationSize: "+populationSize);
		System.out.println("dimension: "+dimension);
		System.out.println("epsilon: "+epsilon);
		System.out.println("destino: "+caminho);
		
		double[][] weights = WeightVectors.initializeUniformlyInTwoDimensions(epsilon, populationSize);
		for(int i=0; i<populationSize; i++){
			Double esquerda = weights[i][0];
			Double direita = weights[i][1];
			String entrada_esquerda = esquerda+"";
			String entrada_direita = direita+"";
			
			double valor_esquerda = Double.parseDouble(entrada_esquerda.replace(',', '.'));
			double valor_direita = Double.parseDouble(entrada_direita.replace(',', '.'));
			Impressora.getInstance().imprimirArquivo(caminho,valor_esquerda+" "+valor_direita+"\n");
			
			//DecimalFormat df = new DecimalFormat("0.000000E0");
			//System.out.println(df.format(esquerda).toLowerCase()+" "+df.format(direita).toLowerCase());
			//Impressora.getInstance().imprimirArquivo(caminho, df.format(esquerda).toLowerCase()+" "+df.format(direita).toLowerCase()+"\n");
			

			
		}
		System.out.println("\n VETOR GERADO COM SUCESSO!");
		}

}
