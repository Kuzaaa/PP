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
    double* vectRes = NULL;
	int n = atoi(argv[1]);
	matA = malloc(n*sizeof(double*));
    vectX = malloc(n*sizeof(double));
    vectTMP = malloc(n*sizeof(double));
    vectRes = malloc(n*sizeof(double));

    for(i=0;i<n;i++){
        matA[i] = malloc(n*sizeof(double));
    }

	for(i=0;i<n;i++){
        for(j=0;j<n;j++){
		      matA[i][j] = rand_a_b(0,100);
		      //printf("matA: %f\n",matA[i][j]);
        }
        vectX[i] = rand_a_b(0,100);
        //printf("vectX: %f\n",vectX[i]);
        vectRes[i] = 0;
	}

    t_start=clock();

	#pragma omp parallel for private(j)
    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectRes[i])
        for(j=0;j<n;j++){
            vectTMP[j] = matA[i][j]*vectX[j];
            vectRes[i] += vectTMP[j];
            //printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(), vectRes[i]);
        }
        //printf("Somme totale : %f\n", vectRes[i]);
    }

	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());

	/*printf("\nmatA = \n");
	for(i=0;i<n;i++){
		printf("[");
		for(j=0;j<n;j++){
			printf("%f,",matA[i][j]);
		}
		printf("]\n");
	}*/

	/*printf("\nvectX = [");
	for(j=0;j<n;j++){
		printf("%f,",vectX[j]);
	}
	printf("]\n\n");*/

	//printf("vectRes = [");
    for(i=0;i<n;i++){
		//printf("%f,",vectRes[i]);
        free(matA[i]);
    }
	//printf("]\n");
	free(matA);

	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

/*******************/
/*   JEU DE TEST   */
/*******************/

//gcc -fopenmp ex5_1.c -o ex5_1
/* ./ex7_a 20 1
 * ./ex7_a 20 10
 * ./ec7_a 1000 1
 * ./ex7_a 1000 10
 *
 * Avec le deuxième argument qui correspond à la taille du tablea, et le troisème au nombre de threads.
 *
 */

 /* Avec 5 threads fixes nous obtenons les performances suivantes :
  *
  * n		Temps d'exécution (en s)
  * 5		0,001091
  * 10		0,001468
  * 20		0,001827
  * 50		0,002006
  * 100		0,001635
  * 200		0,000985
  * 500		0,000557
  * 1000	0,001362
  * 2000	0,012829
  * 5000	0,111531
  * 10000	0.449370
  *
  * Nous pouvons voir que le temps d'exécution augmente en règle général, même s'il y a
  * une baisse de temps lorsque le tableau atteint la centaines d'éléments jusqu'a 500.
  *
  * Avec 500 cases du tableau fixe, et en modifiant le nombre de threads,
  * nous obtenons les performances suivantes :
  *
  * Nombre de threads	Temps d'exécution (en s)
  * 1					0,001263
  * 2					0,004083
  * 5					0,000567
  * 10					0,026670
  * 20					0,006452
  * 50					0,008613
  * 100					0,013563
  * 200					0,017682
  * 500					0,027990
  *
  * Nous pouvons voir que le temps d'exécution augmente en règle général, même s'il y a
  * une baisse de temps lorsque le nombre de thread atteint la dizaine. 
  */
