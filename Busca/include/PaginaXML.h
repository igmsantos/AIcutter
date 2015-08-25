#ifndef PAGINA_XML_H
#define PAGINA_XML_H
#include "Texto.h"
#include "Pagina.h"
#include "LinhaXML.h"
#include "tinyxml.h"

class CPaginaXML : public CPagina
{
public:
	
	TiXmlNode *paginaXML;
	CLinhaXML *linhas, *ultima_linha;
	
	CPaginaXML(TiXmlNode*, CTexto*, int);
	~CPaginaXML();	
	TiXmlNode 	*getPaginaXML();
	CLinhaXML	*getLinhas();
	CPalavra	*getPalavras();
	char		*getTexto(); 
	char 		*getNormalizado();
	int			 getTamanho();
	info_t		*getInfo();
	info_t 		info;
		
};
#endif
