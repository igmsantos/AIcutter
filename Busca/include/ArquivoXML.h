#ifndef ARQUIVO_XML_H
#define ARQUIVO_XML_H

#include "funcoes.h" 

#include "Documento.h"
#include "PaginaXML.h"
#include "LinhaXML.h"
//#include "Bloco.h"
//#include "Linha.h"
//#include "Palavra.h"
#include <tinyxml.h>

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/**
 * A fun��o da classe CArquivoXML � permitir a manipula��o de arquivos XML gerados a partir
 * de documentos em formato PDF, ou qualquer outro formato que seja convertido pr�viamente para
 * XML, com as tags <document>,<page> e <text>.
 * 
 * Para interpretar o XML � utilizada a biclioteca TinyXML, que possue o melhor custo/beneficio
 * em rela��o ao desempenho e necessidades da aplica��o. 
 * 
 * Ao chamar a fun�ao CArquivoXML::getPaginas(), o seguinte processo ocorre:
 * 
 * 
 * CArquivoXML::getPaginasXML()
 *	Para todas as paginas:
 * 		realoca CDocumento::texto com o tamanho atual + tamanho da pagina
 * 		CDocumento::texto = CDocumento::texto + CPagina->getTexto();
 * 		
 * 
 * 
 */

class CArquivoXML : public CDocumento
{
public:
	char *nome;
	char *path;
	char *XML;
	char *arquivo_xml;
	char *arquivo;
	
	CPaginaXML *paginas, *ultima_pagina;
	CLinhaXML  *linhas, *ultima_linha;
	
	TiXmlDocument *ArquivoXML;
	
	CArquivoXML(char*,char*);
	CArquivoXML(char*);
	~CArquivoXML();
	char	*getArquivo();
	char	*getNome();
	char	*getPath();
	char 	*getTexto();
	char 	*getNormalizado();
	//CPagina *getPaginas();
	CPaginaXML  *getPaginas(); 
	CLinhaXML   *getLinhas(); 
	CPalavra    *getPalavras();	
	
	TiXmlDocument *getArquivoXML();
	
	CArquivoXML &setIndice(CLetra*);
	CArquivoXML &Indexar(CPalavra*);
	CLetra *getIndice();
	
};
#endif
