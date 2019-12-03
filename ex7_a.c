#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20
#define M 30

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
		      printf("matA: %f\n",matA[i][j]);
        }
        vectX[i] = rand_a_b(0,100);
        printf("vectX: %f\n",vectX[i]);
        vectRes[i] = 0;
	}

    t_start=clock();

    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectRes[i])
        for(j=0;j<n;j++){
            vectTMP[j]=matA[i][j]*vectX[j];
            vectRes[i]+=vectTMP[j];
            printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(), vectRes[i]);
        }
        printf("Somme totale : %f\n", vectRes[i]);
    }

	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());

    for(i=0;i<n;i++){
        free(matA[i]);
    }
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
/* ./ex5_1 20 30 1
 * ./ex5_1 20 30 10
 *
 * Uniquement pour la version dynamique:
 * ./ex5_1 1000 1500 1
 * ./ex5_1 1000 1500 10
 *
 * Avec le deuxième argument qui correspond à la taille du tableau, et le troisème au nombre de threads.
 *
 * Pour passer du tableau dynamique au tableau statique, il faut commenter les parties du code entre les commentaires
 * //dynamique et //fin dynamique, et enlever les commentaires entre //statique et //fin statique
 * Et vice versa pour repasser au tableau dynamique par la suite.
 */
