#ifndef LETRA_H
#define LETRA_H
#include "Palavra.h"
#include <malloc.h>
class CLetra {
	public:
	CLetra **letras;
	char letra;
	int num_letras;
	int num_palavras;
	int posicao;
	CPalavra *palavras, *ultima_palavra;

	 CLetra(char);
	~CLetra();

	CLetra *getLetra(char *);
	CLetra *addLetra(char *, CPalavra *, int);
	CLetra *setLetra(CLetra*,char *);
	CLetra *addPalavra(CPalavra*);
	void	Listar();
};

#endif
