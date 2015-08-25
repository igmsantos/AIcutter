#ifndef LOBO_H_
#define LOBO_H_

class CLobo;

#include <string.h>
#include "Cortex.h"

/**
 *  	A classe Lobo � responsavel por gerenciar as fun��es de treinamento e utiliza��o da rede neural, e tamb�m de manter a heran�a
 *  de mem�ria entre os documentos iguais de per�odos diferentes.
 */

using namespace FANN;

class CLobo
{
public:
	CCortex		 *cortex;				///< Acesso ao instanciador

	//Rede Neural;
	neural_net   *rede_neural;				///< Rede neural responsavel pela resolu��o do problema a partir das caracteristicas de entrada e respostas de saida.
	unsigned int num_camadas;				///< Quantidade de camadas utilizadas na intercone��o dos neuronios
	unsigned int num_neuronios_invisiveis;	///< Quantidade de neuronios da camada intermediaria
	unsigned int num_neuronios_invisiveis2;	///< Quantidade de neuronios da camada intermediaria
		unsigned int num_caracteristicas;		///< Quantidade de neuronios utilizados na camada de entrada
	unsigned int num_respostas;				///< Quantidade de neuronios utilizados na camada de saida
	unsigned int *camadas;					///< Array com os parametros das camadas
	float 		 conectividade;				///< Porcentagem de conex�es totais entre os neuronios

	//Treinamento
	training_data *dados;					///< Caracteristicas de entrada e saida relacionadas
	unsigned int turnos;					///< Numero de epocas em que as caracteristicas excitam a rede neural formando as liga��es
	unsigned int turnos_por_amostra;		///< Numero de epocas em que as amostras de plotagem e mapas de treinamento s�o gerados
	float margem_de_erro;					///< Diferen�a minima permitida entre cada amostra
	float aprendizagem;						///< Nivel de agressividade em que a aprendizagem � treinada

	char ann_file[FILENAME_MAX]; 		///< Arquivo da rede neural .net
	char train_file[FILENAME_MAX];		///< Arquivo de dados do treinamento inicial .train
	char plot_file[FILENAME_MAX];		///< Arquivo de dados do treinamento para plotagem da curva de aprendizagem .plot
	char map_file[FILENAME_MAX];		///< Arquivo de dados do treinamento de identifica��o .html
	int  grupo;

						 CLobo(CCortex*, training_data*, int); 	///< Construtor

	neural_net 			 *getRedeNeural(); 					///< Cria rede neural baseada no numero de caracteristicas e respostas

	training_data		 *getDados(); 						///< Obtem caracteristicas e respostas
	int 				  getNumDados();					///< Numero de dados
	void 				  analisaDados(); 					///< Analisa caracteristicas atrav�s da rede neural
	void				  generalizaDados();				///< Generaliza caracteristicas atrav�s da rede neural
	void 				  deduzDados();

	fann_type 			**getCaracteristicas(); 				///< Retorna lista de ponteiros de caracteristicas
	CLobo 				 *setCaracteristicas(int, fann_type**); ///< Modifica caracteristicas dos dados

	fann_type 			**getRespostas(); 					///< Retorna lista de ponteiros de respostas
	CLobo 				 *setRespostas(int, fann_type**); 					///< Modifica respostas dos dados


};

#endif /*LOBO_H_*/
