/*
 * Caracteristicas.h
 *
 *  Created on: 24/06/2010
 *      Author: ivan
 */

#ifndef CARACTERISTICAS_H_
#define CARACTERISTICAS_H_

#include <floatfann.h>
#include <fann_cpp.h>
#include <malloc.h>
#include <string.h>

using namespace FANN;

#define CARACTERISTICAS_MAX 128

#define ZERO_SIZE (CARACTERISTICAS_MAX*(sizeof(fann_type)))
extern char zero[ZERO_SIZE] __attribute__(( aligned(sizeof(fann_type)) ));

class CCaracteristicas {
	public:
	CCaracteristicas *next;
	CCaracteristicas *nextOc;
	fann_type *entradas;
	fann_type *saidas;
	int num_entradas;
	int	tamanho_entradas;
	int num_saidas;
	int	tamanho_saidas;
	int ocorrencias;
	CCaracteristicas(int, fann_type *, int, fann_type *);
	int Compara(CCaracteristicas*);
	int Replica();
	int Normaliza();
};

void print_entradas(fann_type*,int);

#endif /* CARACTERISTICAS_H_ */
