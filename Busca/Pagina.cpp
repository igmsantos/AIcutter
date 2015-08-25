#include "include/Pagina.h"
#include "include/funcoes.h" 
#include <stdio.h>
#include <string.h>
#include <malloc.h>
CPagina::CPagina(char *_texto ,CTexto *_parent, int _posicao, size_t _tamanho) : CTexto(_texto,_parent,_posicao,_tamanho)
{
	tamanho = _tamanho;
	linhas = ultima_linha = NULL;
	palavras = ultima_palavra = NULL;
	num_linhas = 0;
	num_palavras = 0;
	index = NULL;
	
	if(!_texto)
	 	return;
	 	
	if( _tamanho < 0 )
		_tamanho = strlen(_texto);

	texto = _texto;
	
}

CPagina::~CPagina(){
	if(linhas)
		while(num_linhas--){
			ultima_linha = linhas;
			linhas=(CLinha*)linhas->getNext();
			delete ultima_linha;
		}
	
	if(index)
		delete index;
		
}

CPagina::CPagina(CTexto *_parent,int _posicao) : CTexto(NULL,_parent,_posicao,-1){
	linhas = ultima_linha = NULL;
	palavras = ultima_palavra = NULL;
	num_linhas = 0;
	num_palavras = 0;
}

CLinha *CPagina::getLinhas(){
	if(linhas)
		return linhas;
	
	int count;
	char *pos;
	char c;
	int tamanho_linha;
	if(!getTexto())
		return NULL;
	
	//ponteiro_t *lista = NULL, *p;
	CLinha *linha = NULL;
	tamanho_linha = 0;
	
	for(num_linhas=count=0;count < tamanho;count++){
	  c = texto[count]; 
	  if(c == '\n' || !c){
	  	if(ultima_linha){
	  		pos =  ultima_linha->getTexto()+ultima_linha->getTamanho();
	  	}else{
	  		pos = texto;
	  	}
	  	tamanho_linha = &texto[count] - pos;
	  	
	  	linha = new CLinha(pos, (CTexto*)this, num_linhas++, tamanho_linha);
	    /* if(ultima_linha)
			printf("%d %d %d %d %d %d\n", pos, &texto[count], ultima_linha->getTexto(), ultima_linha->getTamanho(),tamanho_linha, count);	
	    else
	  		printf("%d %d %d %d %d %d\n", pos, &texto[count],linha->getTexto(), linha->getTamanho(),tamanho_linha, count);	
	  	*/
	  	linha->setNext(*ultima_linha);
	  	linha->setOffset(pos - texto);
	  	ultima_linha = linha;
	  }
	}
	printf("Pagina %d: %d linhas, %d bytes\n", getPosicao(), num_linhas,sizeof(CLinha)*num_linhas);
  	
  	if(!num_linhas)	
		return NULL;
		
	linhas = (CLinha*)malloc(sizeof(CLinha)*num_linhas);
	memset(linhas,0,sizeof(CLinha)*num_linhas);
	while(linha){
		//printf("\n%d %p %d %p\n", linha->getPosicao(),linha->getTexto(),linha->getTamanho(),(char*)&linha->getNext());
		count = linha->getPosicao();
		linhas[count] = *linha;
		ultima_linha = linha;
		linha = (CLinha*)linha->getNext();
		delete ultima_linha;
		if(count < num_linhas - 1)
			linhas[count].setNext((CTexto&)linhas[count+1]);
		else
			linhas[count].setNext();
			
		if(count > 0)
			linhas[count].setPrev((CTexto&)linhas[count-1]);	
		else
			linhas[count].setPrev();	
		
	}
	return linhas;
}
CPalavra *CPagina::getPalavras(){
	if(palavras)
		return palavras;
		
	CLinha *linha = getLinhas();
	CPalavra *palavra;
	ultima_palavra = NULL;
	while(linha){
		palavra = linha->getPalavras();
		if(linha->num_palavras){
		//	printf("%p\n", palavra);
			//printf("%d ", num_palavras);
			num_palavras += linha->num_palavras;
			if(ultima_palavra){
				palavra->setPrev((CTexto&)*ultima_palavra);
				ultima_palavra->setNext((CTexto&)*palavra);
			}else
				palavras = palavra;
				
			ultima_palavra = linha->ultima_palavra; //&palavra[linha->num_palavras-1];
		}
		linha = (CLinha*)linha->getNext();
	}
	
	return palavras;
}

CPagina &CPagina::setLinhas(CLinha *_linhas){
	linhas = _linhas;
	return *this;
}
CPagina &CPagina::addLinha(CLinha &_linha){
	return *this;
}


CPagina &CPagina::Indexar(CPalavra *palavra){
	if(!index)
		index = new CLetra(0);
		
	index->addPalavra(palavra);
	return *this;
}		

CPagina &CPagina::setIndice(CLetra *_index){
	index = _index;
	return *this;
}

CLetra *CPagina::getIndice(){
	if(index)
		return index;
		
	if(!getPalavras())
		return NULL;
	
	index = new CLetra(0);
	int count = 0;
	CPalavra *palavra = palavras;
	for(palavra = palavras; palavra &&  count++ < num_palavras ; palavra = (CPalavra*) palavra->getNext())
		index->addPalavra(palavra);
		
	return index;
}
