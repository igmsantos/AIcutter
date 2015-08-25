#ifndef DOCUMENTO_H
#define DOCUMENTO_H
#include "Texto.h"
#include "Pagina.h"
#include "Bloco.h"
#include "Linha.h"
#include "Palavra.h"
#include "Letra.h"

/**
 * A função da classe documento é englobar todas as outras classes que representam o conteúdo de um documento em texto não formatado.
 * As linhas que possuem quebra de linha com o hífen no final sofrem a remoção do hífen e do caractere de quebra de linha.
 * 
 * A classe CDocumento tem como finalidade principal armazenar o documento em memória com o mínimo de fragmentação dos dados possível, 
 * permitindo uma manipulação lógica dos dados através de funções de acesso e atribuição de cada propriedade das classes inferiores CPagina, 
 * CBloco, CLinha, CPalavra e CLetra.
 * Tais características de alocação são fundamentais para garantir a performance das aplicações que se utilizem da classe CDocumento, 
 * assim preparada para manipular documentos de grande tamanho, com milhares de paginas e linhas, milhões de palavras e bilhões de letras,
 * sem a duplicação da informação em memória, e de forma indexada pelo próprio posicionamento linear.
 * A Codificação de texto é o ISO-8859-1 ou Latin1, que utiliza apenas um byte para cada caracter em uma faixa de 0 a 255 (0x0 <-> 0xFF)
 * optimizando o processo de normalização e busca.
 *   
 * Processo de alocação e maximização do uso da memória:
 * 
 * Quando um documento é criado, seu conteúdo é armazenado em um único bloco de memória com o conteúdo de texto do documento inteiro, as páginas
 * separadas por uma quebra de linha tripla "\n\n\n" (duas linhas em branco), as linhas terminadas por um caracter de quebra de linha "\n", 
 * e as palavras naturalmente separadas por espaço. 
 * 
 * Quando fazemos requisição de uma pagina através do método CDocumento::getPagina(int), obtemos um objeto CPagina contendo na verdade
 * não o texto (string) da pagina, mas sim o endereço de memória aonde a pagina se inicia, e seu tamanho em bytes, sem contar os caracteres de 
 * quebra de linha.
 * Da mesma forma quando fazemos requisição de uma linha através do método CPagina::getLinha(int), obtemos um objeto CLinha contendo na verdade 
 * não o texto (string) da linha, mas sim o endereço de memória aonde a linha se inicia, e seu tamanho em bytes, sem contar o caracter de quebra 
 * de linha, e o mesmo ocorre com a classe inferior CPalavra.
 */

/**
 * TODO
 * 
 * - Implementar possibilidade de configurar o separador utilizado para definir a quebra de pagina, linha, e palavra.
 * 
 * */
class CDocumento : public CTexto
{

public:
	CPagina  *paginas, *ultima_pagina;  ///< Lista de objetos CPagina
	CBloco   *blocos, *ultimo_bloco;   ///< Lista de objetos CBloco
	CLinha   *linhas, *ultima_linha;   ///< Lista de objetos CLinha
	CPalavra *palavras, *ultima_palavra; ///< Lista de objetos CPalavra
	CLetra	 *index;
		
	int num_paginas;	///< Total de objetos CPagina
	int num_blocos;		///< Total de objetos CBloco
	int num_linhas;		///< Total de objetos CLinha
	int num_palavras;	///< Total de objetos CPalavra

	///Construtor do objeto Documento a partir de um ponteiro de memória pré alocado para uma string contendo todo o documento, seguido pelo tamanho em bytes
	CDocumento(char*, size_t);
	
	///Construtor padrão
	CDocumento();
	~CDocumento();
	
	//Aquisicao coletiva de dados dos niveis inferiores (PAGINA, BLOCO, LINHA, PALAVRA)
	  
	/// Retorna todas as paginas identificadas do documento a partir da quebra tripla de linha ou outro separador configurado
	    CPagina    *getPaginas ();	
	  
	/// Retorna os blocos identificados do documento (Recortes)
	    CBloco     *getBlocos  ();
		
	/// Retorna todas as linhas identificadas do documento, a partir do caracter de quebra de linha  
		CLinha     *getLinhas  ();	
		
	/// Retorna todas as palavras identificadas do documento, a partir do caracter de espaço
		CPalavra   *getPalavras();	

	//Aquisicao individual de dados dos niveis inferiores (PAGINA, BLOCO, LINHA, PALAVRA)

	/// Retorna objeto CPagina a partir de um inteiro equivalente ao numero da pagina, ou NULL caso não existente
		CPagina    &getPagina (int);
		
	/// Retorna objeto CBloco a partir de um inteiro equivalente ao numero do bloco, ou NULL caso não existente
		CBloco     &getBloco  (int);
		
	/// Retorna objeto CLinha a partir de um inteiro equivalente ao numero da pagina, e posteriormente o inteiro equivalente ao numero da linha da referida pagina, ou retorna NULL caso não a pagina ou a linha não existam 
		CLinha     &getLinha  (int,int);
		
	/// Retorna objeto CPalavra a partir de um inteiro equivalente ao numero da pagina, posteriormente o inteiro equivalente ao numero da linha da referida pagina e finalmente o inteiro equivalente ao numero da palavra, ou retorna NULL caso a pagina, a linha, ou a palavra não existam
		CPalavra   &getPalavra(int,int,int);

	//Inclusao de dados do nivel inferior (PAGINA, BLOCO)
	
	/// Atribui uma lista de objetos CPagina pré alocadas e definidas
		CDocumento &setPaginas(CPagina *);
		
	///	Adciona um objeto CPagina a lista de paginas	
		CDocumento &addPagina(CPagina&);
		
	/// Atribui uma lista de objetos CBloco pré alocadas e definidas	
		CDocumento &setBlocos(CBloco *);	
	
	/// Adciona um objeto CBloco a uma lista de blocos
		CDocumento &addBloco(CBloco&);
		
		
		CDocumento &setIndice(CLetra*);
		CDocumento &Indexar(CPalavra*);
		CLetra *getIndice();
		
};
#endif
