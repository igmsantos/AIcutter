#ifndef GRIFFON_H_
#define GRIFFON_H_

/** TODO
 *
 * buscaCadernos
 * leCadernos
 * leCaderno
 * analisaLayout
 * identificaLinhas
 * separaRecortes
 * IdentificaRecortes
 *
 */

/**
 *
 *  A classe Griffon � responsavel por englobar as fun��es do ator (usu�rio) ao realizar as tarefas necess�rias
 *  de identifica��o de cabe�alhos e recortes (artigos), e identifica��o dos clientes em que o recorte se refere.
 *	As tarefas foram divididas em subtarefas de acordo com o fluxo de entrada e saida dos processos nos quais o
 *	usu�rio interage com o documento at� a obten��o dos recortes identificados:
 *
 *	- Procurar documentos PDF
 *	- Analisar inicialmente
 *	- Identificar inicialmente as linhas
 *	- Aguardar a verifica��o
 *	- Separar recortes
 *	- Identificar recortes
 *
 */
class CGriffon;

#include "Cerebro.h"
#include "DocumentoIA.h"

class CGriffon{
public:
	CCerebro 	 *cerebro;
	CDocumentoIA *documentos;
	CGriffon();

	CDocumentoIA *buscaDocumentos();
	CDocumentoIA *pegaDocumento(char*);
	int 		  analisaLayout();
	int			  generalizaLayout();
	int			  identificaLinhas();
    CBloco 	     *separaRecortes();
    int 		  identificaRecortes();
    int			  Work(char*);
    int			  Work();
};



#endif /*GRIFFON_H_*/
