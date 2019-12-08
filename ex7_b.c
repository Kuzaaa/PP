#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double rand_a_b(double a,double b);

int main(int argc, char** argv) {
	int m = atoi(argv[2]);
	int i,j,k;
	clock_t t_start,t_end;
	double t_seq;

	srand(time(NULL));
	omp_set_num_threads(m);

	double** matA = NULL;
    double** matB = NULL;
    double** matC = NULL;
    double* vectTMP = NULL;

    double somme = 0;

	int n = atoi(argv[1]);
	matA = malloc(n*sizeof(double*));
    matB = malloc(n*sizeof(double*));
    matC = malloc(n*sizeof(double*));
    vectTMP = malloc(n*sizeof(double));

    for(i=0;i<n;i++){
        matA[i] = malloc(n*sizeof(double));
        matB[i] = malloc(n*sizeof(double));
        matC[i] = malloc(n*sizeof(double));
    }

	#pragma omp parallel for private(j)
	for(i=0;i<n;i++){
        for(j=0;j<n;j++){
		      matA[i][j] = rand_a_b(0,10);
		      matB[i][j] = rand_a_b(0,10);
		      matC[i][j] = 0;
        }
	}

    t_start=clock();

   #pragma omp parallel for private(i,j,k) schedule(static,m)
   for (i=0; i<n; i++){
	  for (j=0; j<n; j++){
		 for (k=0; k<n; k++){
			matC[i][j] += (matA[i][k])*(matB[k][j]);
		 }
	  }
   }


	t_end=clock();

	printf("\nmatA = \n");
	for(i=0;i<n;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%f,",matA[i][j]);
		}
		printf("]\n");
	}

	printf("\nmatB = \n");
	for(i=0;i<n;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%f,",matB[i][j]);
		}
		printf("]\n");
	}

	printf("\nmatC = \n");
	for(i=0;i<n;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%f,",matC[i][j]);
		}
		printf("]\n");
	}

	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());


	for(i=0;i<n;i++){
		free(matA[i]);
		free(matB[i]);
		free(matC[i]);
	}
	free(matA);
	free(matB);
	free(matC);
	free(vectTMP);

	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

/* Avec 5 threads fixes nous obtenons les performances suivantes :
 *
 * n	Temps d'exécution (en s)
 * 
 * 1	0,000083
 * 2	0,000084
 * 5	0,000136
 * 10	0,000132
 * 20	0,000522
 * 50	0,004676
 * 100	0,037335
 * 200	0,284749
 * 500	5,599433
 *
 * Nous pouvons voir que le temps d'exécution augmente linéairement avec 
 * la taille du tableau.
 *
 * Avec n = 200, et en modifiant le nombre de threads, nous obtenons les 
 * performances suivantes :
 *
 * Nombre de threads	Temps d'exécution (en s)
 * 
 * 1					0,285671
 * 2					0,293883
 * 5					0,296978
 * 10					0,29019
 * 20					0,284839
 * 50					0,295942
 * 100					0,320295
 * 200					0,353718
 * 
 * Le temps d'exécution reste assez constant entre 1 et 50 threads.
 * Il augmente légèrement à partir de 100 threads.
 *
 *
 * ****** JEU DE TEST *******
 *
 * gcc -fopenmp ex7_b.c -o ex7_b
 * ./ex7_b 5 5
 * ./ex7_b 100 1
 *
 * Avec le deuxième argument qui correspond à la taille du tableau, et 
 * le troisème au nombre de threads.
 */
