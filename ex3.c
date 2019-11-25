#include <stdio.h>
#include <omp.h>

int i;

#pragma omp private(i)
int main(){
	i = -1;
	#pragma omp parallel private(i)
	{
		printf("La valeur parallèle est %d\n",i);
	}
	printf("La valeur séquentielle est %d\n",i);
}

/*
 * question 1
 * 
 * A l'exécution de ce programme, la 1ère valeur parallèle vaut -1, les 
 * autres valent 0, et la valeur séquentielle vaut -1.
 * Ceci s'explique par l'instruction threadprivate(i) qui ne va pas garder 
 * l'ancienne valeur de i dans les nouveaux threads.
 * 
 * question 2
 * 
 * Si on ajoute copyin(i) dans la commande pragma, la valeur de i sera copiée
 * et donc tous les threads auront une valeur parallèle de -1.
 * 
 * question 3
 * 
 * Quand on met la variable i en privée, i vaut des valeurs différentes 
 * de -1 (j'ai obtenu 0 et 32675), ceci s'explique par le fait que la 
 * dernière valeur de i n'est pas copiée dans la section parallèle.
 */
