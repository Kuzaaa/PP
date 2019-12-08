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
		
		//critique
		/*#pragma omp critical
		{
			total += tab[i];
		}*/
		
		//atomique
		#pragma omp atomic
		total = total + tab[i];
		
		tab[i]=tab[i]*tab[i];
		printf("Thread n° %d, carré : %d \n",omp_get_thread_num(),tab[i]);
	}
	printf("total = %d\n",total);
}

/*
 * a)
 * La variable total ne vaut pas la somme des termes du tableau, car 
 * c'est une variable partagée et donc tous les threads peuvent la 
 * modifier en même temps, ce qui corrompt sa valeur.
 * 
 * b)
 * Quand total est une variable privée, elle vaut sa valeur initiale en
 * fin de boucle car chaque thread la modifie de son côté et elle est 
 * détruite en même temps que le thread.
 * 
 * c)
 * Quand on met une section critique, les performances sont équivalentes
 * à un code non parallèle car un seul thread à la fois peut modifier
 * la variable.
 * */
