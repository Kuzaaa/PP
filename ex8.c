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
		      //printf("matA: %f\n",matA[i][j]);
        }
        vectX[i] = rand_a_b(0,100);
        //printf("vectX: %f\n",vectX[i]);
        vectRes[i] = 0;
	}

    t_start=clock();

	#pragma omp parallel for private(j)
    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectTMP2[i])
        for(j=0;j<n;j++){
            vectTMP[j] = matA[i][j]*vectX[j];
            vectTMP2[i] += vectTMP[j];
            //printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(), vectRes[i]);
        }
        vectTMP2[i] += vectX[i];
        //printf("Somme totale : %f\n", vectRes[i]);
    }

    #pragma omp parallel for private(j)
    for(i=0;i<n;i++){
        #pragma omp parallel for reduction(+:vectRes[i])
        for(j=0;j<n;j++){
            vectTMP[j] = matA[i][j]*vectTMP2[j];
            vectRes[i] += vectTMP[j];
            //printf("Somme partielle du thread n° %d : %f\n",omp_get_thread_num(), vectRes[i]);
        }
        vectRes[i] += vectX[i];
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
	free(vectTMP);
	free(vectX);
	free(vectRes);

	return 0;
}

double rand_a_b(double a,double b){
	return (rand()/(double)RAND_MAX)*(b-a)+a;
}
