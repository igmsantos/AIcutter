/*
 * Dados.h
 *
 *  Created on: 14/07/2010
 *      Author: ivan
 */

#ifndef DADOS_H_
#define DADOS_H_

class CDados {
	int 		 num_dados;
	int 		 num_caracteristicas;
	char 	   **descricao;
	fann_type  **dados_originais;
	fann_type  **dados_escalados;
	fann_type  **dados_generalizados;
	fann_type  **dados_deduzidos;
	fann_type  **dados_externos;
	fann_type  **dados_aleatorios;
	fann_type 	 maximos[CARACTERISTICAS_MAX];
	fann_type 	 minimos[CARACTERISTICAS_MAX];
	fann_type 	 diferencas[CARACTERISTICAS_MAX];

	CDados();
	fann_type *getMaximos();
	fann_type *getMinimos();
	fann_type *getDiferencas();
	fann_type **getDadosOriginais();
	fann_type **getDadosEscalados();
	fann_type **getDadosGeneralizados();
	fann_type **getDadosDeduzidos();
	fann_type **getDadosExternos();
	fann_type **getDadosAleatorios();

	CDados *setDadosOriginais(fann_type**);
	CDados *setDadosEscalados(fann_type**);
	CDados *setDadosGeneralizados(fann_type**);
	CDados *setDadosDeduzidos(fann_type**);
	CDados *setDadosExternos(fann_type**);
	CDados *setDadosAleatorios(fann_type**);

	int getNumDados();
	int getStatus();
	int getNumCaracteristicas();

	CDados *setNumDados(int);
	CDados *setStatus(int);
	CDados *setNumCaracteristicas(int);

};

#endif /* DADOS_H_ */
