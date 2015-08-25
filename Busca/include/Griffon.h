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
 *  A classe Griffon é responsavel por englobar as funções do ator (usuário) ao realizar as tarefas necessárias
 *  de identificação de cabeçalhos e recortes (artigos), e identificação dos clientes em que o recorte se refere.
 *	As tarefas foram divididas em subtarefas de acordo com o fluxo de entrada e saida dos processos nos quais o
 *	usuário interage com o documento até a obtenção dos recortes identificados:
 *
 *	- Procurar documentos PDF
 *	- Analisar inicialmente
 *	- Identificar inicialmente as linhas
 *	- Aguardar a verificação
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
