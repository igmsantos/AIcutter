#ifndef CEREBRO_H_
#define CEREBRO_H_

#define FOCO_LINHAS 	1
#define FOCO_RECORTES 	2

#define LIMITE_NULO 		0
#define LIMITE_ORIGINAL		1
#define LIMITE_RESCALADO    2
#define LIMITE_NORMALIZADO	3
#define LIMITE_DESVIO		4

//#define LIMITE_SIMETRICO	4

#define LOOPS 100
#define BIT_FAIL_COUNTER 100
#define MSE_COUNTER 100

#include <stdio.h>

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	GRAY		7
#define	WHITE		8

#define CLEAR_SCREEN "\033[2J"

class CCerebro;

#include "Caracteristicas.h"
#include "DocumentoIA.h"

//using namespace FANN;

/**
 * O Cerebro obtem as caracteristicas e respostas do meio externo, em que no momento se dispoe.
 * verifica as condicoes externas, apontando o alvo da concentracao e o objeto em tese.
 *
 * As caracteristicas provém originalmente dos problemas enfrentados no cotidiano da tarefa designada.
 * Os problemas modelados são a identificação das linhas para formação dos artigos, e a identifação dos
 * clientes relacionados aos artigos.
 *
 * Para solução dos problemas apresentados foram construidas extruturas similares as naturais, as quais por meio de ligações naturalmente
 * criadas, em forma de redes neurais, controem a solução do problema a partir das caracteristicas apresentadas e respostas equivalentes.
 *
 * - Derivam-se os problemas:
 *   - Classificação das linhas:
 *	   	Analise das caracteristicas de cada linha para definição de um tipo genérico descoberto a partir da repetição de caracteristicas
 * 	   visuais, espaciais ou estatisticas de todas as linhas do documento apresentado. Esta generalização é utilizada para condensar os
 * 	   tipos identificados em tipos basicos iguais aos outros do mesmo grugo, agrupados pela generalização.
 *   - Classificação dos clientes
 *
 * Processo de analise do documento:
 * - Griffon -> Pede ao cerebro para se concentrar no documento ( CCerebro::setAtencao() )
 * - Griffon -> Pede ao cerebro para analisar os dados do doumento ( CCerebro::analisaCaracteristicas() )
 * - Cerebro -> Pede ao cortex para analisar as caracteristicas ( CCortex::analisaCaracteristicas() )
 * - Cortex  -> Pede ao cerebro as caracteristicas doque ele esta com a atenção focada ( CCerebro::getCaracteristicasVisuais(),CCerebro::getCaracteristicasEspaciais(),CCerebro::getCaracteristicasEstatisticas(),CCerebro::getCaracteristicasClassificatorias() )
 * - Cerebro -> Retira as caracteristicas do documento e passa ao cortex ( CDocumentoIA::getCaracteristicasVisuais(),CDocumentoIA::getCaracteristicasEspaciais(),CDocumentoIA::getCaracteristicasEstatisticas(),CDocumentoIA::getCaracteristicasClassificatorias() )
 * - Cortex  -> Aloca bloco e monta os dados para os lobos com as caracteristicas obtidas do cerebro ( CCortex::getDados() )
 * - Cortex  -> Pede aos lobos para analisarem os dados montados ( CCortex::analisaCaracteristicas() )
 * - Lobo    -> Analisa dados montados e treinando a rede neural ( CLobo::analisaDados() )
 * - Cortex  -> Retorna status da analise ( 0 ou 1 ) para o cerebro ( CCortex::analisaCaracteristicas() )
 * - Cerebro -> Retorna status da analise ( 0 ou 1 ) para o griffon ( CCerebro::analisaCaracteristicas() )
 *
 */

class CCerebro {
public:
	CDocumentoIA *documento;

	bool		 cascade;
	int			 offset;
	int 		 num_dados;
	int 		 num_dados_identificados;
	int 		 num_caracteristicas;
	char 	   **descricao;
	fann_type  **dados_originais;
	fann_type  **dados_escalados;
	fann_type  **dados_analisados;
	fann_type  **dados_generalizados;
	fann_type  **dados_deduzidos;
	fann_type  **dados_identificados;
	fann_type  **dados_identificados_escalados;
	fann_type  **dados_externos;
	fann_type  **dados_aleatorios;
	fann_type 	*maximos;
	fann_type 	*minimos;
	fann_type 	*diferencas;
	fann_type   *desvios;
	fann_type   *medias;
	fann_type   *meios;
	fann_type	*totais;
	int			*limites;
	int			*identificacao;

	neural_net   *rede_neural;				///< Rede neural responsavel pela resolução do problema a partir das caracteristicas de entrada e respostas de saida.
	unsigned int num_camadas;				///< Quantidade de camadas utilizadas na interconeção dos neuronios
	unsigned int num_neuronios_invisiveis;	///< Quantidade de neuronios da camada intermediaria
	unsigned int num_neuronios_invisiveis2;	///< Quantidade de neuronios da camada intermediaria
	unsigned int *camadas;					///< Array com os parametros das camadas
	fann_type 	 conectividade;				///< Porcentagem de conexões totais entre os neuronios

	unsigned int turnos;					///< Numero de epocas em que as caracteristicas excitam a rede neural formando as ligações
	unsigned int turnos_por_amostra;		///< Numero de epocas em que as amostras de plotagem e mapas de treinamento são gerados
	fann_type margem_de_erro;					///< Diferença minima permitida entre cada amostra
	fann_type aprendizagem;						///< Nivel de agressividade em que a aprendizagem é treinada

	char ann_file[FILENAME_MAX]; 		///< Arquivo da rede neural .net
	char train_file[FILENAME_MAX];		///< Arquivo de dados do treinamento inicial .train
	char plot_file[FILENAME_MAX];		///< Arquivo de dados do treinamento para plotagem da curva de aprendizagem .plot
	char map_file[FILENAME_MAX];		///< Arquivo de dados do treinamento de identificação .html

	neural_net *getRedeNeural(); 					///< Cria rede neural baseada no numero de caracteristicas e respostas

	fann_type *getMaximos();
	fann_type *getMinimos();
	fann_type *getDiferencas();
	fann_type *getDesvios();
	fann_type *getTotais();
	fann_type *getMedias();
	fann_type *getMeios();
	int		  *getLimites();

	fann_type **getDadosOriginais();
	fann_type **getDadosIdentificados();
	fann_type **getDadosEscalados();
	fann_type **getDadosIdentificadosEscalados();
	fann_type **getDadosAnalisados();
	fann_type **getDadosGeneralizados();
	fann_type **getDadosDeduzidos();
	fann_type **getDadosExternos();
	fann_type **getDadosAleatorios();

	CCerebro *setDadosOriginais(fann_type**);
	CCerebro *setDadosEscalados(fann_type**);
	CCerebro *setDadosAnalisados(fann_type**);
	CCerebro *setDadosGeneralizados(fann_type**);
	CCerebro *setDadosDeduzidos(fann_type**);
	CCerebro *setDadosExternos(fann_type**);
	CCerebro *setDadosAleatorios(fann_type**);

	int getNumDados();
	int getStatus();
	int getNumCaracteristicas();

	CCerebro *setNumDados(int);
	CCerebro *setStatus(int);
	CCerebro *setNumCaracteristicas(int);

	CCerebro();
	CDocumentoIA *getDocumento();

	fann_type **getCaracteristicasVisuais();
	fann_type **getCaracteristicasEspaciais();
	fann_type **getCaracteristicasEstatisticas();
	fann_type **getCaracteristicasClassificatorias();

	CCerebro *setAtencao(CDocumentoIA *,int);
	int	analisaCaracteristicas();
	int generalizaCaracteristicas();
	int	deduzCaracteristicas(int);
	int	reproduzCaracteristicas(int);

	int getNumAmostras();
};

#endif /*CEREBRO_H_*/
