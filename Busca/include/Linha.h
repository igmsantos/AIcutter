#ifndef LINHA_H
#define LINHA_H
#include "Texto.h"
#include "Palavra.h"
#include "Letra.h"
class CLinha : public CTexto
{
public:
	CPalavra *palavras, *ultima_palavra;
	int		 num_palavras;
	CLetra	 *index;
	
		CLinha(char*,CTexto*, int, size_t);
		~CLinha();
	//Aquisio coletiva de dados dos niveis inferiores (PALAVRA)
		CPalavra *getPalavras();

	//Aquisio coletiva de dados dos niveis inferiores (PALAVRA)
		CPalavra *getPalavra(int);

	//Incluso de dados do nivel inferior (PALAVRA)
		CLinha	 &setPalavras(CPalavra*);
		CLinha   &addPalavra(CPalavra*);
		CLinha	 &setIndice(CLetra*);
		CLinha	 &Indexar(CPalavra*);
		
	//Funcoes de retorno (DOCUMENTO, BLOCO)
		CTexto &getDocumento();
		CTexto &getBloco();
		CTexto &getPagina();
		CLetra *getIndice();
			
	//	char*   getNormalizado(); 
		//CPalavra *getNext();		
		
};

#endif
