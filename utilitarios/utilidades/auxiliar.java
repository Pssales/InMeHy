package utilidades;

public class auxiliar {
	
	private static auxiliar uniqueInstance;
	private int lala;
	
	private auxiliar() {}
	
	public static auxiliar getInstance() {
		if(uniqueInstance==null)
			uniqueInstance = new auxiliar();
		return uniqueInstance;
	}
	
	public void chama() {
		lala = lala + 1;
	}
	
	public void getlala() {
		System.out.println(lala/100);
	}
	
	
	public void limpa() {
		lala = 0;
	}
	

}
