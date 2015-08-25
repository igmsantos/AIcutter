#include "include/Texto.h"
#include "include/funcoes.h" 
#include <malloc.h>
CTexto::CTexto(char *_texto ,CTexto *_parent, int _posicao, int _tamanho)
{
	texto   = _texto;
	tamanho = _tamanho;
	parent = _parent;
	posicao = _posicao;
	offset = -1;
	prev = next = NULL;
	normalizado = NULL;
}
CTexto::~CTexto(){
	if(!parent){
		if(texto)
			free(texto);
	//	if(normalizado)
		//	free(normalizado);	
	}
}
CTexto &CTexto::setTexto(char *_texto, int _tamanho){
		
	texto = _texto;
	
	if(_tamanho < 0)
		_tamanho = strlen(texto);

	tamanho = _tamanho;
	
	return *this;
}
CTexto &CTexto::setTexto(){
		//if(texto)
		//	delete texto;
		texto = NULL;
		normalizado = NULL;
		//tamanho = -1;
		return *this;
}
CTexto &CTexto::setNormalizado(char *_normalizado, int _tamanho)
{ 
	normalizado = _normalizado;
	return *this; 
}
CTexto &CTexto::setPosicao(int _posicao)
{
	posicao = _posicao; 
	return *this; 
}
CTexto &CTexto::setParent(CTexto &_parent)
{
	parent = &_parent; 
	return *this; 
}
CTexto &CTexto::setPrev(CTexto &_prev)
{
	prev = &_prev; 
	return *this; 
}
CTexto &CTexto::setNext(CTexto &_next)
{
	next = &_next; 
	return *this; 
}
CTexto &CTexto::setPrev()
{
	prev = NULL; 
	return *this; 
}
CTexto &CTexto::setNext()
{
	next = NULL; 
	return *this; 
}
CTexto &CTexto::setTamanho(int _tamanho)
{ 
	tamanho = _tamanho; 
	return *this; 
}
CTexto &CTexto::setOffset(int _offset)
{ 
	offset = _offset; 
	return *this; 
}
char*   CTexto::getTexto(){ 
	if(texto)
		return texto;
		
	if( parent &&  parent->getTexto() && getOffset() > -1 && getTamanho() > -1){
		texto = parent->texto + offset;
		//printf("Pegando o texto do nivel superior %p\n", texto);
	}	
	return texto; 
	
	}
	
char*   CTexto::getNormalizado(){ 
	if(normalizado)
		return normalizado;
		
	if(parent && parent->getNormalizado() && getOffset() > -1){
		normalizado = this->parent->getNormalizado() + this->getOffset();

		return normalizado;
	}
	if(!normalizado && getTexto() && getTamanho() > -1){
		normalizado = new char[tamanho + 1];
		memcpy(normalizado, texto, tamanho + 1);
		normaliza(normalizado,tamanho);
 	}
	//fwrite(texto,1,tamanho,stdout);
		//			printf(" %d\n", tamanho);
	//fwrite(normalizado,1,tamanho,stdout);
		//			printf(" %d --------------------\n", tamanho);

	return normalizado;
}
int     CTexto::getPosicao()		{ return posicao; }
int  CTexto::getTamanho()		{
	if(tamanho > -1){
		return tamanho; 
	}else if(this->getTexto()){ 
		return tamanho = strlen(texto); 
	}
	return 0;
}

int  CTexto::getOffset()			{ 
	if(offset > -1)
		return offset;
	/*else if(this->parent && this->getTexto() && this->parent->getTexto() )
		offset = this->getTexto() - this->parent->getTexto();*/
	return offset;
}
CTexto *CTexto::getParent()		{ return parent; }
CTexto *CTexto::getPrev()		{ return prev; }
CTexto *CTexto::getNext()		{ return next; }
