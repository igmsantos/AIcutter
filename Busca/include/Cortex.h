#ifndef CORTEX_H_
#define CORTEX_H_

class CCortex;

#include "Cerebro.h"

/**
 *  A classe Cortex � respons�vel pelo enlace das diversas redes neurais encapsuladas pelas classes Lobo.
 *  O c�rtex � responsavel por extrair as caracteristicas Visuais, Espaciais, Estat�sticas e Classificat�rias do documento, e integra-las de forma
 *  que os lobos as utilizem sem a necessidade da manuten��o da mem�ria, ou modifica��es no c�digo no caso da modifica��o do numero
 *  de entradas ou saidas de cada lobo (rede neural).
 * De acordo com as caracteristicas de area, espaco entre a linha anterior, fonte, negrito, italico, letra inicial e final da linha for
 * numero ou letra, qual numero ou qual letra, o "cerebro" � estimulado em treinamento, observando como resultado a largura , altura,
 * posicionamento vertical e horizontal por exemplo. Quando testado com as caracteristicas previamente estimuladas em treinamento, a
 * resposta � uma geometria e posicionamento obtidos atrav�s das caracteristicas marcantes de cada linha, como o negrito, o
 * inicio sempre com a letra P, etc... criando uma linha de largura, altura e posicionamento medianos em rela��o a outras linhas com
 * caracteristicas similares. Esta amostra � utilizada para generalizar os tipos de linha em grupos definidos por suas caracteristicas,
 * para posteriormente serem classificadas.
 *
 * As caracteristicas do documento est�o divididas em grupos:
 *
 * - Caracteristicas Visuais:
 *   - Margem superior
 *   - Margem inferior
 *   - Presen�a de negrito
 *   - Presen�a de italico
 *   - Presen�a de link
 *
 * - Caracteristicas Espaciais:
 *   - Largura
 *   - Altura
 *   - Area
 *   - Posi��o X
 *   - Posi��o Y
 *
 * - Caracteristicas Estatisticas:
 *   - Inicio em numero
 *   - Final em numero
 *   - Quantidade de ocorrencias de cada caracter
 *
 * - Caracteristicas Classificat�rias:
 *   - Tipos de linha
 *
 * - Caracteristicas Seletivas:
 *   - Presen�a do nome do cliente em quest�o
 *   - Presen�a do nome do cliente em quest�o em algum cabe�alho abaixo
 *   - Presen�a do nome de outros clientes com mais palavras na mesma posi��o
 *   - Presen�a de alguma palavra chave que caracterize subdistrito ou comarca
 *   - Presen�a de alguma varia��o do nome do cliente em quest�o (nome de pessoas, etc...)
 *
 * */

class CCortex{

public:
	CCerebro *cerebro;
	CLobo *lobos[CARACTERISTICAS_GRUPOS];					///< Classe que engloba a rede neural respons�vel pela generaliza��o das caracteristicas visuais
	training_data *dados[CARACTERISTICAS_GRUPOS];			///< Dados visuais de treinamento e generaliza��o no lobo visual
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
