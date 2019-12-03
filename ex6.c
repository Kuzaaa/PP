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
	double difference = 0;
	double produit = 1;
	double max = -1;
	double min = 101;
	
	#pragma omp parallel for reduction(+:somme) reduction(-:difference) reduction(*:produit) reduction(max:max) reduction(min:min)
	for(i=0;i<n;i++){
		
		somme += T1D[i];
		printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(),somme);
		difference -= T1D[i];
		printf("Difference partielle du thread n° %d : %f\n",omp_get_thread_num(),difference);
		produit *= T1D[i];
		printf("Produit partiel du thread n° %d : %f\n",omp_get_thread_num(),produit);
		if(T1D[i] > max) {
			max = T1D[i];
		}
		if(T1D[i] < min) {
			min = T1D[i];
		}
	}
	printf("Somme totale : %f\n",somme);
	printf("Différence totale : %f\n",difference);
	printf("Produit total : %f\n",produit);
	printf("Minimum : %f\n",min);
	printf("Maximum : %f\n",max);
	
	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());
	
	free(T1D);
	
	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}
