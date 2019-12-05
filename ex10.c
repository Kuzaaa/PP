#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

omp_lock_t lock;

double calcul(double* array, int length){
	double total = 0.0;
	
	#pragma omp parallel for
	for(int i=0;i<length;i++){
		omp_set_lock(&lock);
		total+=array[i];
		omp_unset_lock(&lock);
	}
	return total;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

int main(){
	double array[1024];
	double total;
	clock_t t_start,t_end;
	double t_seq;
	
	srand(time(NULL));
	
	#pragma omp parallel for
	for(int i=0;i<1024;i++){
		array[i]=rand_a_b(0,100);
	}
	
	t_start = clock();
	
	omp_init_lock(&lock);
	total = calcul(array,1024);
	printf("total : %f \n",total);
	omp_destroy_lock(&lock);
	
	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/omp_get_num_threads(),t_seq/omp_get_num_procs());
	
	return 0;
}
