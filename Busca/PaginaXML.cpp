#include "include/PaginaXML.h"
#include "include/funcoes.h" 
#include <stdio.h>
#include <string.h>
#include <malloc.h>

CPaginaXML::CPaginaXML(TiXmlNode *_paginaXML, CTexto *_parent, int _posicao) : CPagina(NULL,_parent,_posicao,-1){
	linhas = ultima_linha = NULL;
	//palavras = ultima_palavra =  NULL;
	//num_linhas = 0;
	//num_palavras = 0;
	
	paginaXML = _paginaXML;
}

CPaginaXML::~CPaginaXML(){
	//printf("Deletando pagina %d\n", posicao);
	if(linhas)
	while(num_linhas--){
		ultima_linha = linhas;
		linhas=(CLinhaXML*)linhas->getNext();
		delete ultima_linha;
	}

	if(index)
		delete index;
}

TiXmlNode *CPaginaXML::getPaginaXML(){
	return paginaXML;
}

///Adquire os objetos CLinhaXML da paginaXML
CLinhaXML *CPaginaXML::getLinhas(){
	//Se houver linhasXML	
	if(linhas)
		//Retorna linhasXML
		return linhas;

	num_linhas = 0;
	
	//Primeiro nó text do XML é o primeiro filho do nó da paginaXML	
	TiXmlNode *linhaXML = getPaginaXML()->FirstChild("text");
	CLinhaXML *linha = NULL;
		
	//Enquanto houver nós XML
	while(linhaXML){
		//Cria um novo objeto CLinhaXML a partir do nó XML
	  	linha = new CLinhaXML(linhaXML, (CTexto*)this, num_linhas++);
	
		//Se houver uma ultima linhaXML  	
	  	if(ultima_linha){
	  		//proxima linha da ultima linhaXML igual a linha atual
	  		linha->setPrev(*ultima_linha);
	  		ultima_linha->setNext(*linha);
	  	
	  	}else
			//Primeira linha XML da lista igual a linha atual
	  		linhas = linha;
	  	//Ultima linhaXML igul a linha
		ultima_linha = linha;
		//linhaXML = proximo nó XML text
	  	linhaXML = linhaXML->NextSibling("text");
	}
	ultima_linha->setNext();
	//printf("ultima linha %p\n", ultima_linha);
	printf("PaginaXML %d: %d Linhas XML, %d bytes\n", getPosicao(), num_linhas,sizeof(CLinhaXML)*num_linhas);
	return linhas;
}
	/*
  //	printf("PaginaXML %d: %d Linhas XML, %d bytes\n", getPosicao(), num_linhas,sizeof(CLinhaXML)*num_linhas);	
	//Se não houver linhasXML lidas
	if(!num_linhas)
		//Retorna NULL
		return NULL;
	
	//Linha é igual a primeira linha da lista
	linha = linhas;
	int count;
	//Aloca memória para todos os objetos CLinhaXML 
	linhas = (CLinhaXML*)malloc(sizeof(CLinhaXML)*num_linhas);
	//Zera bloco de memória
	memset(linhas,0,sizeof(CLinhaXML)*num_linhas);
	
	//Enquanto houver objetos CLinhaXML
	while(linha){
		//printf("\n%d %p %d %p\n", linha->getPosicao(),linha->getTexto(),linha->getTamanho(),(char*)&linha->getNext());
		//Pega posição de linha para posicionar objeto no vetor
		count = linha->getPosicao();
		//Copia objeto linha para o vetor na posição da linha atual
		linhas[count] = *linha;
		//Copia ponteiro para o ultimo objeto CLinhaXML
		ultima_linha = linha;
		//Linha igual ao proximo objeto CLinhaXML 
		linha = (CLinhaXML*)linha->getNext();
		//Desaloca ultimo objeto CLinha lido 
		delete ultima_linha;
		
		//Se o objeto não for o ultimo objeto do vetor
		if(count < num_linhas - 1)
			//Proximo objeto igual ao objeto posterior do vetor
			linhas[count].setNext(linhas[count+1]);
		else
			//Proximo objeto CLinhaXML não existe
			linhas[count].setNext();
		
		//Se o objeto nao for o primeiro do vetor
		if(count > 0)
			//Objeto anterior igual ao objeto anterior do vetor
			linhas[count].setPrev(linhas[count-1]);
		else
			//Objeto anterior não existe
			linhas[count].setPrev();
	}
	//Retorna lista de objetos CListaXML
	return linhas;
}
*/

CPalavra *CPaginaXML::getPalavras(){
	//Se houver palavras lidas
	if(palavras)
		//Retorna palavras lidas
		return palavras;
	
	//Se não houver texto 
	if(!getTexto() || !getLinhas())
		//retorna NULL
		return NULL;
	
	///Pega paginas
	CLinhaXML *linha = linhas;
	CPalavra *palavra;
	int count;
	count = 0;
	
	///Enquanto houver linhas
	while(linha && count < num_linhas){
		///Pega lista de palavras da linha
		palavra = linha->getPalavras();
		///Se houver palavras
		if(palavra){
			///Soma o numero de palavras da linha com o numero total de palavras da pagina	
			num_palavras += linha->num_palavras;
			//printf("%p %p %d palavras lidas\n", palavra, ultima_palavra, num_palavras);
						
			///Se houver uma ultima lista de palavras
			if(ultima_palavra){
				///Lista de palavras anterior = ultima lista
				palavra->setPrev(*ultima_palavra);
				///Proxima lista da ultima lista de palavras = lista de palavras atual
				ultima_palavra->setNext(*palavra);
			}else
				///inicio da lista total de palavras = primeira lista de palavras da primeira linha
				palavras = palavra;
			
			ultima_palavra = linha->ultima_palavra;
			
			//printf("%d %p %p\n", linha->num_palavras, palavra, ultima_palavra);
			
		}
		count++;
		linha = (CLinhaXML*)linha->getNext();
	}
	/*
	 * palavra = palavras;
	while(palavra){
		fwrite(palavra->getTexto(),1,palavra->getTamanho(),stdout);
		printf(" ");
	}
		*/
	
	return palavras;
}

///Estrai das linhas o texto atribuido na leitura do XML
/**
 * Se já houver lido o texto , retorna texto, caso contrario, 
 * tenta obter o texto a partir do texto do nivel superior (CArquivo)
 * mais o offset em relação ao inicio do documento.
 * Se não houver nivel superior ou não houver offset valido 
 * pega as linhas da paginaXML.
 * 
 * Para todas linhas da paginaXML soma o tamanho da linha ao tamanho total.
 * Aloca memória total para caber todas as linhas em uma pagina,
 * copia texto da linha para a pagina na posição que é o offset,
 * desvincula a linha ao texto inferior,
 * atribui o offset da linha em relação ao inicio da pagina.
 * 
 * */
char	*CPaginaXML::getTexto(){
	//Se já houver lido o texto da pagina
	if(texto)
		//Retorna texto da pagina
		return texto;
	
	//Tenta extrair o texto através do texto do arquivo no offset da pagina
	if(CTexto::getTexto())
		return texto;
		
	//Pega todas as linhas da pagina	
	if(!getLinhas())
		return NULL;
	
	CLinhaXML *linha = linhas;
	//Enquanto houver linhas encadeadas
	while(linha){
		//Soma o tamanho da linha mais o caracter de quebra de linha
		//linha->getTexto();
		
		tamanho += (linha->getTamanho()+1);
		//Linha = Proxima linha
		linha = (CLinhaXML*)linha->getNext();
	}
	char *pos;
	//Aloca memória para todas as linhas da pagina mais outro caracter de quebra de linha
	texto = pos =(char*)malloc(tamanho+1);
	//linha = primeira linha da lista encadeada de linhas
	linha = linhas;
	//Enquanto houver linhas encadeadas
	while(linha){
		//printf("%p %p %p\n\n", this, linha, linha->getTexto());
		if(linha->getTexto()){
			//Copia conteudo de texto da linha para a posição até o final da linha
			memcpy(pos, linha->getTexto(), linha->getTamanho());
			//Marca a posição do inicio da linha em relação ao inicio da pagina
			linha->setOffset(pos - texto);
			//Nova posicao igual a posicao antiga mais o tamanho da linha
			pos += linha->getTamanho();
			//Adciona o caracter de quebra de linha ao final da pagina 
			*pos++ = '\n';
			//Desvingula o texto do XML para receber futuramente o texto do arquivo
			free(linha->getTexto());
			linha->setTexto();
			//Linha igual a proxima linha
		}
		linha = (CLinhaXML*)linha->getNext();
	}
	//*pos = '\0';
	//Retorna texto
	return texto;
} 	

int     CPaginaXML::getTamanho(){
	if(tamanho > -1)
		return tamanho;
	
	if(getTexto())
		CTexto::getTamanho();
	
	return tamanho;
}

char 	*CPaginaXML::getNormalizado(){
	if(normalizado)
		return normalizado;
		
	if(getTexto())
		return normalizado = CPagina::getNormalizado();
		
	return normalizado;
}	


info_t *CPaginaXML::getInfo(){
	//Pega argumentos da linha passados na tag
	char *argumento;
	TiXmlAttribute *pAttrib = getPaginaXML()->ToElement()->FirstAttribute();
	
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
