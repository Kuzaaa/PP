#include <stdio.h>
#include <omp.h>
int main()
{
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		double temps_debut = omp_get_wtime();
		int i;
		for(i=0;i<5;i++)
		{
			printf("iteration %d : Bonjour le monde depuis le thread n°(%d)\n",i,omp_get_thread_num());
		}
		printf("Temps pris: %f\n",omp_get_wtime() - temps_debut);
	}
	
	return 0;
}
