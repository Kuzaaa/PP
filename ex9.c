#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int rand_a_b(int a,int b);
void carre(int* tab,int taille);

int main(int argc, char** argv){
	int taille = atoi(argv[1]);
	int m = atoi(argv[2]);
	int* tab = NULL;
	int i;
	clock_t t_start=clock();

	srand(time(NULL));

	omp_set_num_threads(m);

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
	int total = 0;
	printf("Carré \n");
	#pragma omp parallel for
	for(i=0;i<taille;i++){
		//printf("Nombre de threads : %d \n",omp_get_num_threads());
		total+=tab[i];
		
		tab[i]=tab[i]*tab[i];
		printf("Thread n° %d, carré : %d \n",omp_get_thread_num(),tab[i]);
	}
	printf("total = %d\n",total);
}

/*
 * a)
 * La variable total vaut bien la somme des termes du tableau, */
