#ifndef LINHA_XML_H
#define LINHA_XML_H
#include "Texto.h"
#include "Linha.h"
#include "Palavra.h"
#include "tinyxml.h"

#define DECORACAO_NENHUMA 0
#define DECORACAO_NEGRITO 1
#define DECORACAO_ITALICO 2

class CLinhaXML : public CLinha
{
public:
	TiXmlNode *linhaXML;
	CLinhaXML(TiXmlNode*, CTexto*, int);	
	TiXmlNode	*getLinhaXML();
	char 		*getTexto();
	//char		*get
	int			getTamanho();
	char 		*getNormalizado();
	CPalavra 	*getPalavras();	
	info_t		*getInfo(); 
	int 		decoracao;
	info_t 		info;
	
	
};
#endif
