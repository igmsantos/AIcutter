#include "include/Bloco.h"
#include "include/funcoes.h" 
#include <stdio.h>
#include <string.h>
#include <malloc.h>
CBloco::CBloco(char *_texto ,CTexto *_parent, int _posicao, size_t _tamanho) : CTexto(_texto,_parent,_posicao,_tamanho)
{
	size_t count;
	register char c,l;
	
	if( _tamanho == 0 )
		_tamanho = strlen(_texto);

	texto = _texto;
	l = '.';
	for( tamanho = count = 0 ; count <= _tamanho ; count++ ){
	  c = _texto[count];
	  if(c != '\r')
		  if(c == '\n'){
			  if(l == '-') 
				tamanho--;
			  else if(l != '.' && l != ';' && l != ':')
				texto[tamanho++] = ' ';
			  else
				texto[tamanho++] = '\n';
		  }
		  else
			 texto[tamanho++] = l = c;
		}
	texto[tamanho--] = '\0';
}

CLinha *CBloco::getLinhas(){
	size_t count;
	char *pos;
	char c;
	//ponteiro_t *lista = NULL, *p;
	CLinha *linha, *ultima_linha = NULL;
	for(num_linhas=count=0;count <= tamanho;count++){
	  c = texto[count]; 
	  if(c == '\n' || !c){
	  	if(ultima_linha)
	  		pos =  ultima_linha->getTexto()+ultima_linha->getTamanho()+1;
	  	else
	  		pos = texto;
	  		
	  	linha = new CLinha(pos, (CTexto*)this, num_linhas++,&texto[count] - pos);
	  	linha->setNext(*ultima_linha);
	  	linha->setOffset(count+1);
	  	ultima_linha = linha;
	  }
	}
  	//printf("%d %d\n", num_linhas,sizeof(CLinha)*num_linhas);	
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
		if(count > 0)
			linhas[count].setPrev((CTexto&)linhas[count-1]);
		
	}
	return linhas;
}

CPalavra *CBloco::getPalavras(){
	size_t count;
	CLinha *linha = getLinhas();
	CPalavra *palavras, *ultimas_palavras;
	while(linha){
		palavras = linha->getPalavras();
		if(linha->num_palavras){
			num_palavras += linha->num_palavras;
			palavras->setNext((CTexto&)*ultimas_palavras);
			ultimas_palavras->setPrev((CTexto&)*palavras);
			ultimas_palavras = palavras;
		}
		linha = (CLinha*)linha->getNext();
	}
	return palavras;
}
CBloco &CBloco::setLinhas(CLinha **_linhas){
	return *this;
}

CBloco &CBloco::addLinha(CLinha &_linha){
	return *this;
}