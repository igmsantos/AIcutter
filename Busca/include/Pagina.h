#ifndef PAGINA_H
#define PAGINA_H
#include "Texto.h"
#include "Linha.h"
#include "Palavra.h"
#include "Letra.h"
#include "tinyxml.h"

class CPagina : public CTexto
{
public:
	CLetra	  *index;
	CLinha    *linhas, *ultima_linha; ///< Lista de objetos CLinha
	CPalavra  *palavras, *ultima_palavra;///< Lista de objetos CPalavra
	int        num_linhas;///< Total de objetos CLinha
	int 	   num_palavras;///< Total de objetos CPalavra
	
	///Construtor do objeto CPagina 
		CPagina(char*,CTexto*, int, size_t);
		CPagina(CTexto*, int);	
		~CPagina();
		
	//Aquisicao coletiva de dados dos niveis inferiores (LINHA, PALAVRA)
		CLinha   *getLinhas();	
		CPalavra *getPalavras();	

	//Aquisicao coletiva de dados dos niveis inferiores (LINHA, PALAVRA)
		CLinha	 &getLinha(int);	
		CPalavra &getPalavra(int,int);	
	
	//Inclusao de dados do nivel inferior (LINHA)
		CPagina   &setLinhas(CLinha *);	
		CPagina   &addLinha(CLinha&);

	//Funes de retorno (Documento)
		CTexto &getDocumento();	
		
		CPagina &setIndice(CLetra*);
		CPagina	&Indexar(CPalavra*);
		CLetra *getIndice();
	
};
#endif
