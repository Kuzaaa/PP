#include <stdio.h>
#include <omp.h>

int i;

#pragma omp threadprivate(i)
int main(){
	i = -1;
	#pragma omp parallel
	{
		#pragma omp single copyprivate(i)
		{
			i = 2;
		}
		printf("[thread n°%d]La valeur parallèle est %d\n",omp_get_thread_num(),i);
	}
	printf("La valeur séquentielle est %d\n",i);
}
 
/*
 * En faisant varier le nombre de threads, la valeur de i reste toujours 
 * à 2, cela est dû à la commande copyprivate qui va définir la valeur de 
 * i pour tous les autres threads.
 */
