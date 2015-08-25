/*
 * Griffon.cpp
 *
 *  Created on: 18/06/2010
 *      Author: ivan
 */

#include "include/Griffon.h"

CGriffon::CGriffon(){
	cerebro = new CCerebro();
	documentos = NULL;
}

CDocumentoIA *CGriffon::pegaDocumento(char *documento){
	documentos = new CDocumentoIA(documento);
	return documentos;
}

int CGriffon::identificaLinhas(){
	if(!documentos){
		printf("Sem documentos\n");
		return 0;
	}

	cerebro->setAtencao(documentos,FOCO_LINHAS);
	cerebro->getDadosDeduzidos();
	return 1;
/*
	caracteristicas_t *caracteristicas =
	cerebro->cortex->caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS];

	fann_type **dados = caracteristicas->entradas;

	int num_entradas = caracteristicas->num_entradas;
	//fwrite(linha->getTexto(),1,linha->getTamanho(),stdout);
	//printf("\n");

	int tamanho_entradas = caracteristicas->num_entradas * sizeof(fann_type);

	int num_dados = caracteristicas->num_dados;

	int i,u,count;
	fann_type *entradas, max;

	while(num_dados)
	u = 0;
	i = num_entradas;
	max = 0;
	while(i--)
		if(entradas[i] > max){
			max = entradas[i];
			u = i;
		}

	memset(entradas,0,tamanho_entradas);
	entradas[u] = max;

*/

}


int CGriffon::Work(char *documento){
	printf("Lendo %s...\n", documento);
	pegaDocumento(documento);
	identificaLinhas();
	documentos->reproduzCaracteristicasClassificatorias(cerebro->dados_deduzidos, cerebro->num_dados);
	return 1;
}

int CGriffon::Work(){
	return 1;
}

