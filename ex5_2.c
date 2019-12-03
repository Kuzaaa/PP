#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double rand_a_b(double a,double b);

int main(int argc, char** argv) {
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	int i;
	clock_t t_start,t_end;
	double t_seq;
	
	srand(time(NULL));
	omp_set_num_threads(m);

	double* T1D = NULL;
	
	T1D = malloc(n*sizeof(double));
	
	t_start=clock();
	#pragma omp parallel for
	for(i=0;i<n;i++){
		T1D[i] = rand_a_b(0,100);
		printf("%f\n",T1D[i]);
	}
	
	double somme = 0;
	
	#pragma omp parallel for reduction(+:somme)
	for(i=0;i<n;i++){
		
		somme += T1D[i];
		printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(),somme);
	}
	printf("Somme totale : %f\n",somme);
	
	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());
	
	free(T1D);
	
	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

/* Avec 5 threads fixes, nous obtenons les performances suivantes :
 * 
 * n		Temps (en s)
 * 
 * 5		0,00078
 * 10		0,000962
 * 15		0,00126
 * 20		0,002792
 * 30		0,002058
 * 50		0,003378
 * 75		0,00454
 * 100		0,005825
 * 150		0,008123
 * 200		0,011547
 * 500		0,028389
 * 1000		0,049329
 * 2000		0,10568
 * 5000		0,239569
 * 10000	0,492261
 * 20000	0,956898
 * 50000	2,395036
 * 
 * Avec un tableau de 500 cases, et en faisant varier le nombre de 
 * threads, nous obtenons :
 * 
 * Nombre de threads	Temps (en s)
 * 
 * 1					0,021168
 * 2					0,026179
 * 3					0,026954
 * 5					0,027692
 * 8					0,028835
 * 10					0,026244
 * 15					0,028408
 * 20					0,029655
 * 30					0,030399
 * 50					0,032718
 * 100					0,041426
 * 200					0,053035
 * 300					0,068332
 * 500					0,085399
 * 
 * Le résultat ne semble pas très logique car le temps d'excéution 
 * augmente avec le nombre de threads alors qu'il devrait diminuer.

