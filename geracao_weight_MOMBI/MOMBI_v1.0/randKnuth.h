/* randKnuth.h: Generador de numeros aleatorios, de acuerdo a especificacion de Knuth.
 *
 * Seccion de Computacion - CINVESTAV
 *
 * Introduccion a la Computacion Evolutiva
 * Profesor: Carlos Coello Coello
 * Autor:    Raquel Hernandez Gomez
 * Mayo 2012
 */

void advance_random();
int flip(float prob);
void initrandomnormaldeviate();
double noise(double mu,double sigma);
void randomize(float Rseed);
double randomnormaldeviate();
float randomperc();
int rnd(int low, int high);
float rndreal(float lo ,float hi);
void warmup_random(float random_seed);

