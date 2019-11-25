#include <stdio.h>
#include <omp.h>
#define X 10
int main(){
	int i = -1;
	printf("Je suis le thread main du no: %2d\n",omp_get_thread_num());
	printf("La valeur de la variable locale à main dans la région séquentielle est: %2d\n",i);
	omp_set_num_threads(5);
	#pragma omp parallel firstprivate(i)
	{
		int j;
		i = omp_get_thread_num();
		for(j=0;j<X;j++){
			printf("La valeur de la variable locale à main dans la région parallèle est: %2d\n",i);
		}
	}
	printf("Je suis le thread main du numéro: %2d\n",omp_get_thread_num());
	printf("La valeur de la variable locale à main dans la région séquentielle est: %2d\n",i);
	
	return 0;
}

/*question 1
 * 
 * Chaque thread est exécuté une fois et fait 2 tour de boucle parallèle.
 * 
 * Avant la région  parallèle, la variable i vaut -1, pendant i vaut le numéro du thread exécuté 
 * et après, i vaut le numéro du dernier thread exécuté.
 * 
 * question 2
 * 
 * En modifiant X, nous pouvons observer que l'affichage n'est pas réparti équitablement entre les threads.
 * La variable i est modifiée par un autre thread avant que le premier thread ne l'affiche.
 * 
 * question 3
 * 
 * Il suffit de mettre la variable i en privée dans la section parallèle:
 * #pragma omp parallel firstprivate(i)
 */
