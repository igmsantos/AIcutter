#include <string.h>
#include "include/Documento.h"
#include "include/funcoes.h" 

CDocumento::CDocumento(char *_texto ,size_t _tamanho) : CTexto(_texto,NULL,0,_tamanho)
{
	size_t count;
	register char c,l;
	linhas 	= ultima_linha = NULL; 
	paginas = ultima_pagina = NULL;
	blocos 	= ultimo_bloco = NULL;
	palavras = ultima_palavra = NULL;
	index = NULL;
	num_paginas = NULL;
	num_blocos 	= NULL;
	num_linhas 	= NULL;
	num_palavras = NULL;
	
	//Se não ouver ponteiro de texto cria o objeto padrao vazio.
	if(!_texto)
	 	return;
	
	//Se o valor do argumento _tamanho for igual a 0, busca o tamanho do _texto
	if( _tamanho == 0 )
		//Tamanho inicial = ao tamanho da string até o terminador /0
		_tamanho = strlen(_texto);

	//Copia ponteiro _texto
	texto = _texto;
	
	l = '.';
	//Enquanto não chegar no final da string
	for( tamanho = count = 0 ; count <= _tamanho ; count++ ){
	  c = _texto[count];
	  //Se o caracter em questão for uma quebra de linha
	  if(c == '\n'){
	  	  //Caso o ultimo caracter seja um ifem
		  if(l == '-') 
			//Volta um caracter
			tamanho--;
		  //ou se diferente de um ponto final
		  else if(l != '.')
			//substitue quebra de linha por espaço		
			texto[tamanho++] = ' ';
		  //ou	
		  else
		    //o caracter é mesmo uma quebra de linha
			texto[tamanho++] = '\n';
	  }
	  //ou se não for caracter de quebra de linha
	  else
	  	 //o caracter faz parte do texto
		 texto[tamanho++] = l = c;
	}
	//Finaliza a string com o ultimo caracter terminador /0
	texto[tamanho--] = '\0'; 
}

CDocumento::~CDocumento(){
	//printf("Deletando documento %p\n", paginas);

	while(paginas && num_paginas--){
		ultima_pagina = paginas;
		paginas = (CPagina*)paginas->getNext();
		delete ultima_pagina;
		
	}
	if(index)
		delete index;	
}

CBloco *CDocumento::getBlocos(){
	return blocos;
}

CDocumento &CDocumento::setBlocos(CBloco *_blocos){
	blocos = _blocos;
	return *this;
}

CDocumento &CDocumento::addBloco(CBloco &_bloco){
	return *this;
}

CPagina *CDocumento::getPaginas(){
	return paginas;
}
/*	size_t count;
	char *pos;
	char a, b = 0;
	//ponteiro_t *lista = NULL, *p;
	CPagina *pagina, *ultima_pagina = NULL;
	for(num_paginas=count=0;count <= tamanho;count++){
	  a = texto[count];
	  if(!b && a)
	  	pos = &texto[count];
	  else if(!a && b){
	  	palavra = new CPalavra(pos, (CTexto*)this, num_palavras++,&texto[count] - pos);
	  	palavra->setNext(*ultima_palavra);
	  	palavra->setOffset(count+1);
	  	ultima_palavra = palavra;
	  }
	}
  	//printf("%d %d\n", num_linhas,sizeof(CLinha)*num_linhas);	
	palavras = (CPalavra*)malloc(sizeof(CPalavra)*num_palavras);
	memset(palavras,0,sizeof(CPalavra)*num_palavras);
	while(palavra){
		//printf("\n%d %p %d %p\n", linha->getPosicao(),linha->getTexto(),linha->getTamanho(),(char*)&linha->getNext());
		count = palavra->getPosicao();
		palavras[count] = *palavra;
		ultima_palavra = palavra;
		palavra = (CPalavra*)&palavra->getNext();
		delete ultima_palavra;
		if(count < num_palavras - 1)
			palavras[count].setNext((CTexto&)palavras[count+1]);
		if(count > 0)
			palavras[count].setPrev((CTexto&)palavras[count-1]);
		
	}
	return palavras;

}
*/

/// Cria uma lista encadeada de objetos CLinha na ordem correta, a partir das listas de objetos CLinha pre-alocados pelas paginas em CPagina::getLinhas() 
CLinha *CDocumento::getLinhas(){
	//Se as linhas já tiverem sido lidas
	if(linhas)
		//retorna linhas
		return linhas;
		
	if(!getPaginas())
		return NULL;
	
	//Pega paginas
	CPagina *pagina = getPaginas();
	CLinha *linha;
	//Enquanto houver paginas
	while(pagina){
		//Pega lista de linhas da pagina
		linha = pagina->getLinhas();
		//Se houver linhas
		if(linha){
			//Soma o numero de linhas da pagina com o numero total de linhas do documento
			num_linhas += pagina->num_linhas;
			//Se ja houver uma ultima lista de linhas
			if(ultima_linha){
				//Lista anterior = Ultima lista 
				linha->setPrev(*ultima_linha);
				//Proxima lista da ultima lista = lista atual 
				ultima_linha->setNext(*linha);
			}else
				// lista total de linhas = primeira lista de linhas da primeira pagina
				linhas = linha;
				 
			ultima_linha = pagina->ultima_linha;
		}
		//Pagina = proxima pagina
		pagina = (CPagina*)pagina->getNext();
	}
	
	return linhas;
}

/// Cria uma lista encadeada de objetos CPalavra na ordem correta, a partir das listas de objetos CPalavra pre-alocados pelas linhas em CLinha::getPalavras() e encadeadas em CPagina::getPalavras() 
CPalavra *CDocumento::getPalavras(){
	
	if(palavras)
		return palavras;
	
	 printf("procurando palavras...");
	
	if(!getPaginas())
		return NULL;
	
	///Pega paginas
	CPagina *pagina = getPaginas();
	CPalavra *palavra;
	///Enquanto houver paginas
	while(pagina){
		///Pega lista de palavras da pagina
		palavra = pagina->getPalavras();
		///Se houver palavras
		if(palavra){
			///Soma o numero de palavras da pagina com o numero total de palavras do documento	
			num_palavras += pagina->num_palavras;
			///Se houver uma ultima lista de palavras
			if(ultima_palavra){
				///Lista de palavras anterior = ultima lista
				palavra->setPrev((CTexto&)*ultima_palavra);
				///Proxima lista da ultima lista de palavras = lista de palavras atual
				ultima_palavra->setNext((CTexto&)*palavra);
			}else
				///inicio da lista total de palavras = primeira lista de palavras da primeira pagina
				palavras = palavra;
				
			ultima_palavra = pagina->ultima_palavra;
		}
		///pagina = proxima pagina
		pagina = (CPagina*)pagina->getNext();
	}
	
	return palavras;
}


CDocumento &CDocumento::Indexar(CPalavra *palavra){
	if(!index)
		index = new CLetra(0);
		
	index->addPalavra(palavra);
	return *this;
}		

CDocumento &CDocumento::setIndice(CLetra *_index){
	index = _index;
	return *this;
}

CLetra *CDocumento::getIndice(){
	if(index)
		return index;
		
	if(!getPalavras())
		return NULL;
	
	index = new CLetra(0);
	int count = num_palavras;
	CPalavra *palavra = palavras;
	for(palavra = palavras; count-- ; palavra = (CPalavra*) palavra->getNext())
		index->addPalavra(palavra);
		
	return index;
}
