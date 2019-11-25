#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20

double rand_a_b(double a,double b);

int main(int argc, char** argv) {
	int m = atoi(argv[2]);
	int i;
	clock_t t_start,t_end;
	double t_seq;
	
	srand(time(NULL));
	omp_set_num_threads(m);
	
	//statique
	/*double T1D[N];
	
	t_start=clock();
	#pragma omp parallel for
	for(i=0;i<N;i++){
		T1D[i] = rand_a_b(0,100);
		printf("%f\n",T1D[i]);
	}*/
	
	//dynamique
	double* T1D = NULL;
	int n = atoi(argv[1]);
	T1D = malloc(n*sizeof(double));
	
	t_start=clock();
	#pragma omp parallel for
	for(i=0;i<n;i++){
		T1D[i] = rand_a_b(0,100);
		printf("%f\n",T1D[i]);
	}
	
	
	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());
	
	//dynamique
	free(T1D);
	
	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}
