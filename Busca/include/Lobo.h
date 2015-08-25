#ifndef LOBO_H_
#define LOBO_H_

class CLobo;

#include <string.h>
#include "Cortex.h"

/**
 *  	A classe Lobo é responsavel por gerenciar as funções de treinamento e utilização da rede neural, e também de manter a herança
 *  de memória entre os documentos iguais de períodos diferentes.
 */

using namespace FANN;

class CLobo
{
public:
	CCortex		 *cortex;				///< Acesso ao instanciador

	//Rede Neural;
	neural_net   *rede_neural;				///< Rede neural responsavel pela resolução do problema a partir das caracteristicas de entrada e respostas de saida.
	unsigned int num_camadas;				///< Quantidade de camadas utilizadas na interconeção dos neuronios
	unsigned int num_neuronios_invisiveis;	///< Quantidade de neuronios da camada intermediaria
	unsigned int num_neuronios_invisiveis2;	///< Quantidade de neuronios da camada intermediaria
		unsigned int num_caracteristicas;		///< Quantidade de neuronios utilizados na camada de entrada
	unsigned int num_respostas;				///< Quantidade de neuronios utilizados na camada de saida
	unsigned int *camadas;					///< Array com os parametros das camadas
	float 		 conectividade;				///< Porcentagem de conexões totais entre os neuronios

	//Treinamento
	training_data *dados;					///< Caracteristicas de entrada e saida relacionadas
	unsigned int turnos;					///< Numero de epocas em que as caracteristicas excitam a rede neural formando as ligações
	unsigned int turnos_por_amostra;		///< Numero de epocas em que as amostras de plotagem e mapas de treinamento são gerados
	float margem_de_erro;					///< Diferença minima permitida entre cada amostra
	float aprendizagem;						///< Nivel de agressividade em que a aprendizagem é treinada

	char ann_file[FILENAME_MAX]; 		///< Arquivo da rede neural .net
	char train_file[FILENAME_MAX];		///< Arquivo de dados do treinamento inicial .train
	char plot_file[FILENAME_MAX];		///< Arquivo de dados do treinamento para plotagem da curva de aprendizagem .plot
	char map_file[FILENAME_MAX];		///< Arquivo de dados do treinamento de identificação .html
	int  grupo;

						 CLobo(CCortex*, training_data*, int); 	///< Construtor

	neural_net 			 *getRedeNeural(); 					///< Cria rede neural baseada no numero de caracteristicas e respostas

	training_data		 *getDados(); 						///< Obtem caracteristicas e respostas
	int 				  getNumDados();					///< Numero de dados
	void 				  analisaDados(); 					///< Analisa caracteristicas através da rede neural
	void				  generalizaDados();				///< Generaliza caracteristicas através da rede neural
	void 				  deduzDados();

	fann_type 			**getCaracteristicas(); 				///< Retorna lista de ponteiros de caracteristicas
	CLobo 				 *setCaracteristicas(int, fann_type**); ///< Modifica caracteristicas dos dados

	fann_type 			**getRespostas(); 					///< Retorna lista de ponteiros de respostas
	CLobo 				 *setRespostas(int, fann_type**); 					///< Modifica respostas dos dados


};

#endif /*LOBO_H_*/
