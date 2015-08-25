#ifndef BLOCO_H
#define BLOCO_H
#include "Texto.h"
#include "Linha.h"
#include "Palavra.h"

class CBloco : public CTexto
{
public:
	CLinha   *linhas;
	CPalavra **palavras;
	int      num_linhas;
	int 	 num_palavras;
	
		CBloco(char*,CTexto*, int, size_t);

	//Aquisicao coletiva de dados dos niveis inferiores (LINHA, PALAVRA)
		CLinha   *getLinhas();	
		CPalavra *getPalavras();	

	//Aquisicao coletiva de dados dos niveis inferiores (LINHA, PALAVRA)
		CLinha	 &getLinha(int);	
		CPalavra &getPalavra(int,int);	
	
	//Inclusao de dados do nivel inferior (LINHA)
		CBloco   &setLinhas(CLinha **);	
		CBloco   &addLinha(CLinha&);

	//Funes de retorno (Documento)
		CTexto &getDocumento();	
};

#endif
