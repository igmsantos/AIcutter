/*
 * Dados.cpp
 *
 *  Created on: 14/07/2010
 *      Author: ivan
 */
#include "include/Dados.h"

CDados::CDados(){
	int 		 num_dados;
	int 		 num_caracteristicas;
	char 	   **descricao;
	fann_type  **dados_originais;
	fann_type  **dados_aleatorios;
	fann_type  **dados_escalados;
	fann_type  **dados_generalizados;
	fann_type  **dados_externos;
	fann_type  **dados_deduzidos;
	fann_type  *maximos;
	fann_type  *minimos;
	fann_type  *diferencas;

	num_dados = 0;
	num_caracteristicas = 0;
	dados_originais = 0;
	dados_escalados = 0;
	dados_generalizados = 0;
	dados_deduzidos = 0;
	dados_externos = 0;
	dados_aleatorios = 0;

	maximos = minimos = diferencas = NULL;
}

fann_type *CDados::getMaximos(){
	if(maximos)
		return maximos;

	if(!getDadosEscalados())
		return NULL;

	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	maximos = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);
	memset(maximos,0,num_caracteristicas * sizeof(fann_type));

	while(u--)
		maximos[u] = 0;

	count=num_dados;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados_escalados[count];
		while(u--){
			if(caracteristicas[u] > maximos[u])
				maximos[u] = caracteristicas[u];
			else if(caracteristicas[u] < maximos[u])
				maximos[u] = caracteristicas[u];
		}
	}
	printf("\nMáximos:\t");
	print_entradas(maximos,num_caracteristicas);

	return maximos;
}

fann_type *CDados::getMinimos(){
	if(minimos)
		return minimos;

	if(!getDadosEscalados())
		return NULL;

	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	minimos = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);
	memset(minimos,0,num_caracteristicas * sizeof(fann_type));

	while(u--)
		minimos[u] = 0;

	count=num_dados;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados_escalados[count];
		while(u--){
			if(caracteristicas[u] > minimos[u])
				minimos[u] = caracteristicas[u];
			else if(caracteristicas[u] < minimos[u])
				minimos[u] = caracteristicas[u];
		}
	}

	printf("\nMínimos:\t");
	print_entradas(minimos,num_caracteristicas);

	return minimos;

}

fann_type *CDados::getDiferencas(){
	if(diferencas)
		return diferencas;

	if(!getMaximos() || !getMinimos())
		return NULL;

	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	diferencas = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	while(u--)
		diferencas[u] = maximos[u] - minimos[u];

	printf("\nDiferenças:\t");
	print_entradas(diferencas,num_caracteristicas);

	return diferencas;

}

fann_type **CDados::getDadosOriginais(){
	return dados_originais;
}

fann_type **CDados::getDadosEscalados(){
	if(dados_escalados)
		return dados_escalados;

	if(!getDadosOriginais())
		return NULL;

	fann_type *caracteristicas;
	dados_escalados = (fann_type**)malloc(sizeof(fann_type)*num_dados);
	caracteristicas = (fann_type*)malloc(sizeof(fann_type)*num_dados*num_caracteristicas);

	int i,u,count;
	count = num_dados;
	while(count--){
		dados_escalados[count] = caracteristicas;
		u=num_caracteristicas;
		while(u--)
			caracteristicas[u] = (caracteristicas[u] - minimos[u]) / diferencas[u];
	}
	return dados_escalados;
}

fann_type **CDados::getDadosGeneralizados(){

}

fann_type **CDados::getDadosDeduzidos();
fann_type **CDados::getDadosExternos();
fann_type **CDados::getDadosAleatorios();

CDados *CDados::setDadosOriginais(fann_type**);
CDados *CDados::setDadosEscalados(fann_type**);
CDados *CDados::setDadosGeneralizados(fann_type**);
CDados *CDados::setDadosDeduzidos(fann_type**);
CDados *CDados::setDadosExternos(fann_type**);
CDados *CDados::setDadosAleatorios(fann_type**);

int CDados::getNumDados();
int CDados::getStatus();
int CDados::getNumCaracteristicas();

CDados *CDados::setNumDados(int);
CDados *CDados::setStatus(int);
CDados *CDados::setNumCaracteristicas(int);

