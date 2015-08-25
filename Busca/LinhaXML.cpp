#include "include/LinhaXML.h"
#include "include/funcoes.h" 
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <regex.h>

const char vazio[] = "\0";


CLinhaXML::CLinhaXML(TiXmlNode *_linhaXML, CTexto *_parent, int _posicao) : CLinha(NULL,_parent,_posicao,-1){
	linhaXML = _linhaXML;
	decoracao = 0;
	
	memset(&info,-1,sizeof(info));
	info.negrito=0;
	info.italico=0;
	
}

TiXmlNode *CLinhaXML::getLinhaXML(){
	return linhaXML;
}

CPalavra *CLinhaXML::getPalavras(){
	if(palavras)
		return palavras;
	
	if(getTexto())
		CLinha::getPalavras();	
	
	return palavras;
}

info_t *CLinhaXML::getInfo(){
	
	if(!getLinhaXML())
		return NULL;
	//Pega argumentos da linha passados na tag
	char *argumento;
	
	TiXmlAttribute *pAttrib = linhaXML->ToElement()->FirstAttribute();
	
	//printf("\n");
	while (pAttrib)
	{
		argumento = (char*)pAttrib->Name();
		switch(*argumento){
			case 't': 
				pAttrib->QueryIntValue(&info.top);
				break;
			case 'l':
				pAttrib->QueryIntValue(&info.left);
				break;
			case 'w':
				pAttrib->QueryIntValue(&info.width);
				break;
			case 'h':
				pAttrib->QueryIntValue(&info.height);
				break;
			case 'f':
				pAttrib->QueryIntValue(&info.font);
				break;		
		}
		info.bottom = info.top + info.height;
		info.right  = info.left + info.width;
		info.area   = info.height * info.width;
		
		pAttrib=pAttrib->Next();
	}
/*
	printf("t%-4d b%-4d l%-4d r%-4d w%-4d h%-4d a%-8d f%-4d n%-4d i%-4d\n", info.top, info.bottom,
															 info.left, info.right, 
															 info.width,info.height, 
															 info.area, info.font,
															 info.negrito, info.italico);
*/
	return &info;
	
}

int	 CLinhaXML::getTamanho(){
	if(tamanho > -1)
		return tamanho;
		
	if(getTexto())
		CTexto::getTamanho();
	return tamanho;
	
}

///Extrai o texto de uma tag text do XML
char *CLinhaXML::getTexto(){
	//Se houver texto lido
	if(texto)
		//Retorna texto lido
		return texto;
	
	//Tenta extrair o texto através do texto da pagina no offset da linha
	if(CTexto::getTexto())
		return texto;
	
	int  tamanho_texto = 0;

	/**
	 * Procurar texto em todos os nós abaixo da tag text por tags do tipo Text
	 */	
	char *argumento;
	char *xml = NULL;
	TiXmlNode *nivel1 = NULL;
	TiXmlNode *nivel2 = NULL;
	TiXmlNode *nivel3 = NULL;
	if(!getLinhaXML())
		return NULL;
	nivel1 = linhaXML->FirstChild();
	
	//PRIMEIRO NIVEL TAG TEXT
	//Enquanto houver tags dentro da tag text
	tamanho = 0;
	while( nivel1 ){
		//Se a tag for do tipo TEXT
		if(nivel1->Type() == TiXmlNode::TINYXML_TEXT){
			xml = (char*)nivel1->Value();
			if(xml){
				tamanho_texto = strlen(xml);
	//			printf("nivel1: %d %d %p %s\n", tamanho, tamanho_texto, texto, xml);
				// Se ainda não a texto
				if(!texto){
					//Duplica a string
					texto = (char*)malloc(tamanho_texto+1);
					memcpy(texto,xml,tamanho_texto);
				}else{
					//Realoca o bloco da linha para o novo tamanho
					texto = (char*)realloc(texto, tamanho+tamanho_texto + 1);
					//Copia o pedaco do texto
					memcpy(texto+tamanho, xml, tamanho_texto);
				}
				//Soma o tamanho da string encontrada ao tamanho total
				tamanho += tamanho_texto;
			}
			
		}
		//SEGUNDO NIVEL TAG b ou TAG i (negrito ou italico)
		//Ou se a tag for do tipo ELEMENT
		else if(nivel1->Type() == TiXmlNode::TINYXML_ELEMENT){
			argumento = (char*)nivel1->Value();
			if(*argumento == 'b')	 
				info.negrito = 1;
			else if(*argumento == 'i')
				info.italico = 1;
					
			//Pega primeira tag filha
			nivel2 = nivel1->FirstChild();
			//Enquanto houver tags filhas
			while( nivel2 ){
				//Se a tag filha for um texto 
				if(nivel2->Type() == TiXmlNode::TINYXML_TEXT){
				// Se ainda não a texto
					xml = (char*)nivel2->Value();
					if(xml){
						tamanho_texto = strlen(xml);
					//	printf("nivel2: %d %d %p %s\n", tamanho, tamanho_texto, texto, xml);
						// Se ainda não a texto
						if(!texto){
							//Duplica a string
							texto = (char*)malloc(tamanho_texto+1);
							memcpy(texto,xml,tamanho_texto);
						}else{
							//Realoca o bloco da linha para o novo tamanho
							texto = (char*)realloc(texto, tamanho+tamanho_texto + 1);
							//Copia o pedaco do texto
							memcpy(texto+tamanho, xml, tamanho_texto);
						}
						//Soma o tamanho da string encontrada ao tamanho total
						tamanho += tamanho_texto;
					}
				}	
				//TERCEIRO NIVEL TAG B ou TAG i (negrito ou italico) 
				//Ou se a tag for do tipo ELEMENT
				else if(nivel2->Type() == TiXmlNode::TINYXML_ELEMENT){	 
					argumento = (char*)nivel1->Value();
					if(*argumento == 'b')	 
						info.negrito = 1;
					else if(*argumento == 'i')
						info.italico = 1;
			
					//Pega primeira tag filha
					nivel3 = nivel2->FirstChild();
					//Enquanto houver tags filhas
					while( nivel3 ){
						//Se a tag filha for um texto 
						if(nivel3->Type() == TiXmlNode::TINYXML_TEXT){
							//Se houver string 
							xml = (char*)nivel3->Value();
							if(xml){
								tamanho_texto = strlen(xml);
				//				printf("nivel3: %d %d %p %s\n", tamanho, tamanho_texto, texto, xml);
								// Se ainda não a texto
								if(!texto){
									//Duplica a string
									texto = (char*)malloc(tamanho_texto+1);
									memcpy(texto,xml,tamanho_texto);
								}else{
									//Realoca o bloco da linha para o novo tamanho
									texto = (char*)realloc(texto, tamanho+tamanho_texto + 1);
									//Copia o pedaco do texto
									memcpy(texto+tamanho, xml, tamanho_texto);
								}
								//Soma o tamanho da string encontrada ao tamanho total
								tamanho += tamanho_texto;
							}
						}	
						nivel3 = nivel3->NextSibling();
					}
				}
				nivel2 = nivel2->NextSibling();
			}		
		}
		nivel1 = nivel1->NextSibling();
	}
	if(!texto)
		texto = (char*)malloc(1);
		 	
	texto[tamanho] = '\0';	 
	return texto;
}

char *CLinhaXML::getNormalizado(){
	if(!getTexto())
		return NULL;
	return CTexto::getNormalizado();
}
