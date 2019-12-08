#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 20
#define M 30

double rand_a_b(double a,double b);

int main(int argc, char** argv) {
	int m = atoi(argv[3]);
	int i,j;
	clock_t t_start,t_end;
	double t_seq;

	srand(time(NULL));
	omp_set_num_threads(m);

	//statique
	/*double T2D[N][M];

	t_start=clock();
	#pragma omp parallel for
	for(i=0;i<N;i++){
        #pragma omp parallel for
        for(j=0;j<M;j++){
		     T2D[i][j] = rand_a_b(0,100);
		     printf("%f\n",T2D[i][j]);
        }
	}*/
	//fin statique

	//dynamique
	double** T2D = NULL;
	int nl = atoi(argv[1]);
    int nc = atoi(argv[2]);
	T2D = malloc(nl*sizeof(double*));
    for(i=0;i<nl;i++){
        T2D[i] = malloc(nc*sizeof(double));
    }

	t_start=clock();
	#pragma omp parallel for
	for(i=0;i<nl;i++){
        #pragma omp parallel for
        for(j=0;j<nc;j++){
		      T2D[i][j] = rand_a_b(0,100);
		      printf("%f\n",T2D[i][j]);
        }
	}
	//fin dynamique


	t_end=clock();
	t_seq = (float)(t_end-t_start)/CLOCKS_PER_SEC;
	printf("Temps séquentiel : %f \nCharge d'un thread : %f\nCharge d'un processeur : %f\n",t_seq,t_seq/m,t_seq/omp_get_num_procs());

	//dynamique
    for(i=0;i<nl;i++){
        free(T2D[i]);
    }
	free(T2D);
	//fin dynamique

	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}

/*******************/
/*   JEU DE TEST   */
/*******************/

//gcc -fopenmp ex5_3.c -o ex5_3
/* ./ex5_3 20 30 1
 * ./ex5_3 20 30 10
 *
 * Uniquement pour la version dynamique:
 * ./ex5_3 1000 1500 1
 * ./ex5_3 1000 1500 10
 *
 * Avec le deuxième argument qui correspond au nombre de ligne, le 
 * troisième au nombre de colonnes, et le quatrième au nombre de threads.
 *
 * Pour passer du tableau dynamique au tableau statique, il faut commenter les parties du code entre les commentaires
 * //dynamique et //fin dynamique, et enlever les commentaires entre //statique et //fin statique
 * Et vice versa pour repasser au tableau dynamique par la suite.
 */
