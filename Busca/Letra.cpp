#include "include/Letra.h"
#include "stdio.h"

CLetra::CLetra(char _letra){
	letras = (CLetra**)malloc(sizeof(CLetra*)*0xFF);
	memset(letras,0,sizeof(CLetra*)*0xFF);
	palavras = NULL;
	num_letras = 0;
	num_palavras = 0;
	letra = _letra;
	posicao = 0;
}

CLetra::~CLetra(){
	if(num_letras){
		int count;
		for(count=0; count < 0xFF; count++)
			if(letras[count]){
				delete letras[count];
				//printf("deletando %02d-%c\n", posicao, letra);
			}
	}
	free(letras);
}

CLetra *CLetra::getLetra(char *_letra){
	return letras[(unsigned char)*_letra];
}

CLetra *CLetra::addLetra(char *_letra, CPalavra *palavra, int nivel){
	posicao = nivel;
	if(nivel < palavra->tamanho){	
		//printf("%c",*_letra);
		if(!letras[(unsigned char)*_letra])
			letras[(unsigned char)*_letra] = new CLetra(*_letra);//(CLetra*)malloc(sizeof(CLetra*)*0xFF);
	
				 
		letras[(unsigned char)*_letra]->addLetra(_letra+1, palavra, nivel+1);
		num_letras++;
	}
	else{
		//printf("\n");
		if(palavras){
			ultima_palavra->setNextOc(*palavra);
			palavra->setPrevOc(*ultima_palavra);
		}else
			palavras = palavra;
		
		num_palavras++;
		ultima_palavra = palavra;
	}
		
	return letras[(unsigned char)*_letra];
}

CLetra *CLetra::addPalavra(CPalavra *palavra){
	char *_letra = palavra->getNormalizado();
	addLetra(_letra, palavra, 0);
		
	return *letras;
}

void 	CLetra::Listar(){
  static int nivel = -1;
  static int palavras_unicas = 0;
  static int palavras_total = 0;
  static char buff[255];
  int count = 0;
  
 buff[nivel] = letra;
 if(num_palavras){
  	printf("%-2d %-3d %-4d|", nivel, posicao, num_palavras);
 	fwrite(buff,1,nivel,stdout);
  	
 	//while(count++ < nivel)
 		//printf(" ");
 	printf("%c", letra);
 	printf("\n");
  	palavras_unicas++;
  	palavras_total += num_palavras;
 } 
  
  
  if(num_letras){ 
  	count=0;
  	nivel++;
 	while(count < 0xFF){
  		if(letras[count]){ 
 			letras[count]->Listar(); 		
  		}
  		count++;	
  	}
  	nivel--;
  }
  if(nivel == -1)
  	printf("%d palavras unicas, %d palavras total\n", palavras_unicas, palavras_total);
}


