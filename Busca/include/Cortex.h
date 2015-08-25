#ifndef CORTEX_H_
#define CORTEX_H_

class CCortex;

#include "Cerebro.h"

/**
 *  A classe Cortex é responsável pelo enlace das diversas redes neurais encapsuladas pelas classes Lobo.
 *  O córtex é responsavel por extrair as caracteristicas Visuais, Espaciais, Estatísticas e Classificatórias do documento, e integra-las de forma
 *  que os lobos as utilizem sem a necessidade da manutenção da memória, ou modificações no código no caso da modificação do numero
 *  de entradas ou saidas de cada lobo (rede neural).
 * De acordo com as caracteristicas de area, espaco entre a linha anterior, fonte, negrito, italico, letra inicial e final da linha for
 * numero ou letra, qual numero ou qual letra, o "cerebro" é estimulado em treinamento, observando como resultado a largura , altura,
 * posicionamento vertical e horizontal por exemplo. Quando testado com as caracteristicas previamente estimuladas em treinamento, a
 * resposta é uma geometria e posicionamento obtidos através das caracteristicas marcantes de cada linha, como o negrito, o
 * inicio sempre com a letra P, etc... criando uma linha de largura, altura e posicionamento medianos em relação a outras linhas com
 * caracteristicas similares. Esta amostra é utilizada para generalizar os tipos de linha em grupos definidos por suas caracteristicas,
 * para posteriormente serem classificadas.
 *
 * As caracteristicas do documento estão divididas em grupos:
 *
 * - Caracteristicas Visuais:
 *   - Margem superior
 *   - Margem inferior
 *   - Presença de negrito
 *   - Presença de italico
 *   - Presença de link
 *
 * - Caracteristicas Espaciais:
 *   - Largura
 *   - Altura
 *   - Area
 *   - Posição X
 *   - Posição Y
 *
 * - Caracteristicas Estatisticas:
 *   - Inicio em numero
 *   - Final em numero
 *   - Quantidade de ocorrencias de cada caracter
 *
 * - Caracteristicas Classificatórias:
 *   - Tipos de linha
 *
 * - Caracteristicas Seletivas:
 *   - Presença do nome do cliente em questão
 *   - Presença do nome do cliente em questão em algum cabeçalho abaixo
 *   - Presença do nome de outros clientes com mais palavras na mesma posição
 *   - Presença de alguma palavra chave que caracterize subdistrito ou comarca
 *   - Presença de alguma variação do nome do cliente em questão (nome de pessoas, etc...)
 *
 * */

class CCortex{

public:
	CCerebro *cerebro;
	CLobo *lobos[CARACTERISTICAS_GRUPOS];					///< Classe que engloba a rede neural responsável pela generalização das caracteristicas visuais
	training_data *dados[CARACTERISTICAS_GRUPOS];			///< Dados visuais de treinamento e generalização no lobo visual
	caracteristicas_t *caracteristicas[CARACTERISTICAS_GRUPOS];

	int num_dados;				///< Quantidade de entradas contendo as caracteristicas
	int caracteristicas_total;	///< Numero total de grupos de caracteristicas (visuais, espaciais... )


	CCortex(CCerebro*);	/// Construtor

	CLobo *getLobo(int);
	CLobo *getLoboVisual();
	CLobo *getLoboEspacial();
	CLobo *getLoboEstatistico();
	CLobo *getLoboClassificatorio();

	training_data *getDados(int);
	training_data *getDadosOld(int);
	training_data *getDadosVisuais();
	training_data *getDadosEspaciais();
	training_data *getDadosEstatisticos();
	training_data *getDadosClassificatorios();
	training_data *getDadosGeneralizados(int);

	caracteristicas_t **getCaracteristicas();
	int generalizaCaracteristicas();
	int analisaCaracteristicas();
	int deduzCaracteristicas(int);

	CCerebro *getCerebro();
};

#endif /*CORTEX_H_*/
