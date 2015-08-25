// Busca.cpp : Defines the entry point for the console application.
#include <string.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "include/Griffon.h"

using namespace std;

char show_letra(CLetra *l){
  int count = 0;
  static int nivel = -1;
  static int palavras_unicas = 0;
  static int palavras_total = 0;
  
  static char buff[255];
  char letra = l->letra;
  CLetra **letras;
  count = 0;
  buff[nivel] = letra;
 if(l->num_palavras){
  	printf("%-2d %-3d %-4d|", nivel, l->posicao, l->num_palavras);
 	fwrite(buff,1,nivel,stdout);
  	
 	//while(count++ < nivel)
 		//printf(" ");
 	printf("%c", letra);
 	printf("\n");
  	palavras_unicas++;
  	palavras_total += l->num_palavras;
 } 
  
  
  if(l->num_letras){ 
  	count=0;
  	nivel++;
 	letras = l->letras;
  	while(count < 0xFF){
  		if(letras[count]){ 
 			show_letra(letras[count]); 		
  		}
  		count++;	
  	}
  	nivel--;
  }
  if(nivel == -1)
  	printf("%d palavras unicas, %d palavras total\n", palavras_unicas, palavras_total);
}

int main(int argc, char* argv[])
{
 //CDocumentoIA *arquivo;
 // CPalavra    *palavra;
  //CLetra	  *index;
  //arquivo = new CDocumentoIA(argv[1]);
  // pagina  = arquivo->getPaginas();
  // linha = arquivo->getLinhasXML();
   //palavra = arquivo->getPalavras();
  // printf("%d\n", arquivo->num_palavras);
  // printf("%s\n\n\n%s", arquivo->getTexto(), arquivo->getNormalizado());
  
  //arquivo->identificaBlocos();
  //index = new CLetra(0);
  /*while(palavra){
  	//printf("%p ", palavra);
  	index->addPalavra(palavra);
    //	fwrite(palavra->getNormalizado(),1,palavra->getTamanho(), stdout);
  	//printf(" ");
  	palavra = (CPalavra*)palavra->getNext();
  }*/
  	//arquivo->getIndice();
  	
  	//index->Listar();
  	
  	//arquivo->identificaLinhas();
  	
  	//arquivo->analisaLayout();
  	//arquivo->geraHTML(NULL);
    CGriffon *griffon = new CGriffon();
	 //printf("%p\n",griffon->buscaDocumentos());
	griffon->Work(argv[1]);


  //	delete arquivo;
	//show_letra(&index);
   return 0;
}

