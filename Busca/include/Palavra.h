#ifndef PALAVRA_H
#define PALAVRA_H

#include "Texto.h"

typedef  float(*comparador_t)(char*,char*,int,int);
class CPalavra : public CTexto{
public:
	comparador_t comparador;
	float    comparacao;
	CPalavra *variacoes;
	CPalavra *prev_oc;
	CPalavra *next_oc;

		CPalavra(char*,CTexto*, int, size_t);

	//Funcoes internas de comparacao de strings
		float		Compara(CPalavra&);
		CPalavra	*setComparador(comparador_t);
		CPalavra	*setComparacao(float);

		comparador_t getComparador();
		float	 getComparacao();

	//Funcoes de retorno (DOCUMENTO, BLOCO, LINHA)
		CTexto &getDocumento();
		CTexto *getBloco();
		CTexto *getLinha();
	
		CPalavra  &setLinha(CTexto &); 

		CPalavra  &setPrevOc();
		CPalavra  &setNextOc();
		
		CPalavra  &setPrevOc(CPalavra &);
		CPalavra  &setNextOc(CPalavra &);
};	

#endif
