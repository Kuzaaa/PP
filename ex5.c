#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int rand_a_b(int a,int b);
void carre(int* tab,int taille);

int main(int argc, char** argv){
	int taille = atoi(argv[1]);
	int* tab = NULL;
	int i;
	clock_t t_start=clock();

	srand(time(NULL));

	//omp_set_num_threads(10);

	tab=malloc(taille*sizeof(int));

	printf("Nb de coeurs : %d \n",omp_get_num_procs());

	printf("tableau \n");
	#pragma omp parallel for
	for(i=0;i<taille;i++){
		//printf("Nombre de threads : %d \n",omp_get_num_threads());
		tab[i]=rand_a_b(0,100);
		printf("%d \n",tab[i]);
	}
	carre(tab,taille);
	free(tab);

	printf("Temps écoulé : %f secondes \n",(float)(clock()-t_start)/CLOCKS_PER_SEC);

	return 0;
}

int rand_a_b(int a,int b){
	return rand()%(b-a)+a;
}

void carre(int* tab,int taille){
	int i;
	printf("Carré \n");
	#pragma omp parallel for
	for(i=0;i<taille;i++){
		//printf("Nombre de threads : %d \n",omp_get_num_threads());
		tab[i]=tab[i]*tab[i];
		printf("%d \n",tab[i]);
	}
}


/*
 * 5.d) Peu importe la taille du tableau, le nombre de threads vaut 12.
 *
 * 5.e) Pour 1000 valeurs dans le tableau :
 * 		Avec 1 thread : 0.006756 s
 * 		Avec 5 threads : 0.020754 s
 * 		Avec 10 threads : 0.058417 s
 *
 *		On remarque que, plus il y a de threads, plus le programme met du temps à s'éxecuter.
 *
 * 5.f) Il suffit de lancer la ligne de commande "export OMP_NUM_THREADS=10"
 * 		dans le terminal pour changer le nombre de threads à 10
 */

//JEU DE TEST//
// gcc -fopenmp ex5.c -o ex5
/* ./ex5 10
 * ./ex5 1000
 *
 * Avec le deuxième argument qui correspond à la taille du tableau
 */
