#include "include/Linha.h"
#include "include/Bloco.h"
#include "include/funcoes.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

CLinha::CLinha(char *_texto ,CTexto *_parent, int _posicao, size_t _tamanho) : CTexto(_texto,_parent,_posicao,_tamanho)
{
	palavras = NULL;
	ultima_palavra = NULL;
	num_palavras = NULL;
	index = NULL;
}

CLinha::~CLinha(){
	//printf("Deletando linha %d, %d\n", posicao, num_palavras);
		
	if(palavras)
		while(num_palavras--){
			ultima_palavra = palavras;
			palavras = (CPalavra*)palavras->getNext();
			delete ultima_palavra;
		}	
	if(index)
		delete index;
}
CPalavra  *CLinha::getPalavras(){
	
	if(palavras)
		return palavras;
	
	int count;
	char *pos;
	char a, b = 0;
	
	num_palavras = 0;
	
	if(!getNormalizado())	
		return NULL;

	CPalavra *palavra = NULL;
	b = ' ';
	//printf("%d %d %d %d %p ", getPagina().getPosicao(), posicao, tamanho, strlen(texto), texto);
	//fwrite(texto,1,tamanho,stdout);
	//printf("\n");
	pos = NULL;
	for(count=0;count < tamanho;count++){
	  a = normalizado[count];
	 	//printf("%c %c\n", normalizado[count], texto[count]);
	  if(b == ' ' && a != ' ')
	  	pos = &texto[count];
	  else if(a == ' ' && b != ' '){
	  	palavra = new CPalavra(pos, (CTexto*)this, num_palavras++,&texto[count] - pos);
	  	if(ultima_palavra){
	  		ultima_palavra->setNext(*palavra);
	  		palavra->setPrev(*ultima_palavra);
	  	}else
	  		palavras = palavra;	
	  
	  	palavra->setOffset(pos - texto);
	  	ultima_palavra = palavra;
	  //	fwrite(palavra->getTexto(),1, palavra->getTamanho(),stdout);
	  	//printf("\n");
	  	pos = NULL;	
	  }
	  b = a;
	}
	if(pos){
	  	palavra = new CPalavra(pos, (CTexto*)this, num_palavras++,&texto[count] - pos);
	  	if(ultima_palavra){
	  		ultima_palavra->setNext(*palavra);
	  		palavra->setPrev(*ultima_palavra);
	  	}else
	  		palavras = palavra;	
	  
	  	palavra->setOffset(pos - texto);
	  	ultima_palavra = palavra;
	}
	/*printf("\n %d \n", num_palavras);
	palavra = palavras;
	while(palavra){
		fwrite(palavra->getTexto(),1,palavra->getTamanho(),stdout);
		printf(" ");
		palavra = (CPalavra*)palavra->getNext();
	}*/
	return palavras;
}
	/*
*/
	//printf("%d palavras, %d bytes\n", num_palavras,sizeof(CLinha)*num_palavras);	
	/*
	if(!num_palavras)
		return NULL;
		
  	palavras = (CPalavra*)malloc(sizeof(CPalavra)*num_palavras);
	memset(palavras,0,sizeof(CPalavra)*num_palavras);
	while(palavra){
		count = palavra->getPosicao();
		palavras[count] = *palavra;
		ultima_palavra = palavra;
		palavra = (CPalavra*)palavra->getNext();
		delete ultima_palavra;
		if(count < num_palavras - 1)
			palavras[count].setNext((CTexto&)palavras[count+1]);
		else
			palavras[count].setNext();
			
		if(count > 0)
			palavras[count].setPrev((CTexto&)palavras[count-1]);
		else
			palavras[count].setPrev();
	}
	
	return palavras;
}
*/

CLinha	 &CLinha::setPalavras(CPalavra *_palavras){
	return *this;
}

CLinha   &CLinha::addPalavra(CPalavra *_palavra){
	return *this;
}

CTexto 	 &CLinha::getPagina(){
	return *parent;
}

/*
char*   CLinha::getNormalizado(){ 
	if(normalizado)
		return normalizado;
		
	if(parent && getOffset())
		normalizado = parent->getNormalizado() + getOffset();
	
	if(!normalizado){
		normalizado = new char[tamanho+1];
		memcpy(normalizado, texto, tamanho+1);
		normaliza(normalizado,tamanho);
	}	
	return normalizado;
}
*/

CLinha &CLinha::Indexar(CPalavra *palavra){
	if(!index)
		index = new CLetra(0);
		
	index->addPalavra(palavra);
	return *this;
}		

CLinha &CLinha::setIndice(CLetra *_index){
	index = _index;
	return *this;
}

CLetra *CLinha::getIndice(){
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
