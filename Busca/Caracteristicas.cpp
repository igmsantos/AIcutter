/*
 * Caracteristicas.cpp
 *
 *  Created on: 24/06/2010
 *      Author: ivan
 */
#include "include/Caracteristicas.h"

char zero[ZERO_SIZE];// __attribute__(( aligned(sizeof(fann_type)) ));

void print_entradas(fann_type *entradas, int num_entradas){
	int count = 0;
	for(count=0;count<num_entradas;count++){
		printf("%-2.2f ", entradas[count]);
	}
	printf("\n");
}

CCaracteristicas::CCaracteristicas(int _num_entradas, fann_type *_entradas, int _num_saidas, fann_type *_saidas){
	next 	 = NULL;
	nextOc	 = NULL;

	entradas = _entradas;
	num_entradas  = _num_entradas;
	tamanho_entradas = sizeof(fann_type) * num_entradas;

	saidas   	   = _saidas;
	num_saidas 	   = _num_saidas;
	tamanho_saidas = sizeof(fann_type) * num_saidas;


	ocorrencias = 1;
}

int CCaracteristicas::Compara(CCaracteristicas *comparada){
	fann_type *saidas_comparadas = comparada->saidas;

	if( //*(fann_type*)saidas == *(fann_type*)saidas_comparadas &&
		! memcmp(saidas, saidas_comparadas, tamanho_saidas) ){

		fann_type *entradas_comparadas = comparada->entradas;

		int u =  num_entradas;
		 while(u--)
			if(entradas[u])
				entradas[u] = (fann_type)(entradas[u] + saidas_comparadas[u]) / 2;
			else
				entradas[u] = (fann_type)(saidas[u] + saidas_comparadas[u]) / 2;
		 //printf("%p ", entradas);
			//printf("a ");print_saidas(entradas_comparadas,num_entradas);
			//printf("b ");print_saidas(entradas,num_entradas);

		 //else{
		 comparada->nextOc = nextOc;
		 nextOc = comparada;
		 ocorrencias++;
		 //}
		return 1;
	}
	return 0;
}

/*
int CCaracteristicas::Normaliza(){
	CCaracteristicas *tmp = nextOc;
	printf("Normalizando %d saidas...\n", ocorrencias);
	//printf("%p ", saidas);
	//print_entradas(entradas,num_entradas);
	//print_entradas(saidas,num_saidas);
	int i,u;
	fann_type max;

	u = 0;
	i = num_saidas;
	max = 0;
	while(i--)
		if(saidas[i] > max){
			max = saidas[i];
			u = i;
		}

	memset(saidas,0,tamanho_saidas);
	saidas[u] = max;
	//print_entradas(saidas,num_saidas);

	for(tmp=nextOc;tmp;tmp=tmp->nextOc)
		memcpy(tmp->saidas,saidas,tamanho_saidas);

	return 0;
}
*/
int CCaracteristicas::Replica(){
	CCaracteristicas *tmp = nextOc;
	if(ocorrencias  == 1)
		return 0;

	printf("Replicando %d saidas... ", ocorrencias);
	printf("\n");
	//print_entradas(entradas,num_entradas);
	//print_entradas(saidas,num_saidas);

	//for(tmp=nextOc;tmp;tmp=tmp->nextOc)
		//memcpy(tmp->entradas,entradas,tamanho_entradas);

	return 0;
}

