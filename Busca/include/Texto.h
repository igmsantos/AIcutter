#ifndef TEXTO_H
#define TEXTO_H
#include<string.h>

typedef struct {
		int top;
		int left;
		int bottom;
		int right;
		int width;
		int height;
		int area;
		int font;
		int italico;
		int negrito;
		int link;
		int tipo;
}info_t;
	
	
class CTexto 
{

public:
	CTexto  *parent;
	CTexto	*next;
	CTexto  *prev;
	char	*texto;
	char	*normalizado;
	int		posicao;
	int offset;
	int	tamanho;
	 
				 CTexto(char*,CTexto *, int, int);
				~CTexto();
	
		CTexto  &setNormalizado(char*, int);
		CTexto	&setTexto(char*, int);
		CTexto	&setTexto();
		CTexto	&setPosicao(int);
		CTexto	&setOffset(int);
		CTexto	&setTamanho(int);
		CTexto	&setParent(CTexto &);
		CTexto	&setPrev(CTexto &);
		CTexto	&setNext(CTexto &);
		CTexto	&setPrev();
		CTexto	&setNext();
		
		char	*getNormalizado();
		char    *getTexto();
		int	     getPosicao();
		int	     getTamanho();
		int	     getOffset();
		CTexto	*getParent();
		CTexto	*getPrev();
		CTexto	*getNext();	

};

#endif
