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

    for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			somme = 0;
			#pragma omp parallel for reduction(+:somme)
			for(k=0;k<n;k++){
				vectTMP[k] = matA[i][k]*matB[k][j];
				somme += vectTMP[k];
				//printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(), somme);
			}
			//printf("Somme totale : %f\n", somme);
			matC[i][j] = somme;
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
 * 1	0,000091
 * 2	0,000342
 * 5	0,00227
 * 10	0,008965
 * 20	0,037873
 * 50	0,235857
 * 100	0,956156
 * 200	3,77397
 * 500	21,436975
 * 
 * 
 * Nous pouvons voir que le temps d'exécution augmente 
 * linéairement avec la taille du tableau.
 * 
 * Avec n = 100, et en modifiant le nombre de threads,
 * nous obtenons les performances suivantes :
 * 
 * Nombre de threads	Temps d'exécution (en s)
 * 1					0,043819
 * 2					0,270086
 * 5					0,942986
 * 10					1,948223
 * 20					4,059723
 * 50					10,262927
 * 100					21,568026

 * 
 * Le temps d'exécution augmente linéairement avec l'augmentation du
 * nombre de threads, cela ne semble pas très logique.
 * 
 * 
 * ****** JEU DE TEST *******
 * 
 * gcc -fopenmp ex7_b.c -o ex7_b
 * ./ex7_b 5 5
 * ./ex7_b 100 1
 *
 * Avec le deuxième argument qui correspond à la taille du tableau, et le troisème au nombre de threads.
 */
