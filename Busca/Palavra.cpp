#include "include/Bloco.h"
#include "include/funcoes.h"
#include <stdexcept>
using namespace std;
CPalavra::CPalavra(char *_texto ,CTexto *_parent, int _posicao, size_t _tamanho) : CTexto(_texto,_parent,_posicao,_tamanho)
{
	variacoes = NULL;
	comparador = NULL;
	comparacao = 0.0;
	next_oc = NULL;
	prev_oc = NULL;
	
}
CPalavra* CPalavra::setComparador(comparador_t _comparador)
{ 
	comparador = _comparador; 
	return this; 
}
CPalavra* CPalavra::setComparacao(float _comparacao)
{
	comparacao = _comparacao; 
	return this;
}
comparador_t CPalavra::getComparador()	{ return comparador; }
float   CPalavra::getComparacao()		{ return comparacao; }	
float	CPalavra::Compara(CPalavra &comparada)
{

	return comparacao = comparador(getNormalizado(),comparada.getNormalizado(),
		tamanho,comparada.tamanho);	
}
CTexto  *CPalavra::getBloco(){	return parent->getParent(); }
CTexto  *CPalavra::getLinha(){	return parent; }
CPalavra  &CPalavra::setLinha(CTexto &linha){ 
	parent = &linha;
	return *this;
}

CPalavra &CPalavra::setPrevOc()
{
	prev_oc = NULL; 
	return *this; 
}
CPalavra &CPalavra::setNextOc()
{
	next_oc = NULL; 
	return *this; 
}

CPalavra &CPalavra::setPrevOc(CPalavra &_prev)
{
	prev_oc = &_prev; 
	return *this; 
}
CPalavra &CPalavra::setNextOc(CPalavra &_next)
{
	next_oc = &_next; 
	return *this; 
}