#include <stdio.h>
#include <time.h>
#include <stdlib.h>

double rand_a_b(double a,double b);

int main(int argc, char** argv) {
	int m = atoi(argv[2]);
	int i,j;
	clock_t t_start,t_end;
	double t_seq;

	srand(time(NULL));
	omp_set_num_threads(m);

	double** matA = NULL;
    double* vectX = NULL;
    double* vectTMP = NULL;
    double* vectTMP2 = NULL;
    double* vectRes = NULL;
	int n = atoi(argv[1]);
	matA = malloc(n*sizeof(double*));
    vectX = malloc(n*sizeof(double));
    vectTMP = malloc(n*sizeof(double));
    vectTMP2 = malloc(n*sizeof(double));
    vectRes = malloc(n*sizeof(double));

    for(i=0;i<n;i++){
        matA[i] = malloc(n*sizeof(double));
    }

	for(i=0;i<n;i++){
        for(j=0;j<n;j++){
		      matA[i][j] = rand_a_b(0,100);
        }
        vectX[i] = rand_a_b(0,100);
        vectRes[i] = 0;
	}

    t_start=clock();

	#pragma omp parallel for private(j)
    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectTMP2[i])
        for(j=0;j<n;j++){
            vectTMP[j] = matA[i][j]*vectX[j];
            vectTMP2[i] += vectTMP[j];
        }
        vectTMP2[i] += vectX[i];
    }

    #pragma omp parallel for private(j)
    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectRes[i])
        for(j=0;j<n;j++){
            vectTMP[j] = matA[i][j]*vectTMP2[j];
            vectRes[i] += vectTMP[j];
        }
        vectRes[i] += vectX[i];
    }

	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());

	printf("\nmatA = \n");
	for(i=0;i<n;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%f,",matA[i][j]);
		}
		printf("]\n");
	}

	printf("\nvectX = [");
	for(j=0;j<n;j++){
		printf("%f,",vectX[j]);
	}
	printf("]\n\n");

	printf("vectRes = [");
    for(i=0;i<n;i++){
		printf("%f,",vectRes[i]);
        free(matA[i]);
    }
	printf("]\n");
	free(matA);
	free(vectTMP);
	free(vectX);
	free(vectRes);

	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

/* Avec 5 threads fixes nous obtenons les performances suivantes :
 *
 * n	Temps d'exécution (en s)
 * 1	0,000320
 * 2	0,000400
 * 5	0,001802
 * 10	0,001862
 * 20	0,001980
 * 50	0,000629
 * 100	0,000811
 * 200	0,001249
 * 500	0,002243
 *
 * Nous pouvons voir que le temps d'exécution augmente
 * avec la taille du tableau, malgré une chute du temps
 * d'execution aux alentours des n = 50.
 *
 * Avec n = 500, et en modifiant le nombre de threads,
 * nous obtenons les performances suivantes :
 *
 * Nombre de threads	Temps d'exécution (en s)
 * 1					0,002535
 * 2					0,001733
 * 5					0,003247
 * 10					0,002845
 * 20					0,011190
 * 50					0,015525
 * 100					0,024035
 * 200					0,022381
 * 5000					0,043302
 *
 * Le temps d'exécution reste assez constant entre 1 et 10 threads.
 * Il augmente à partir de 50 threads.
 *
 *
 * ****** JEU DE TEST *******
 *
 * gcc -fopenmp ex8.c -o ex8
 * ./ex8 5 5
 * ./ex8 100 1
 *
 * Avec le deuxième argument qui correspond à la taille du tableau, et le troisème au nombre de threads.
 */
