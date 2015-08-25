#include "include/ArquivoXML.h"

CArquivoXML::CArquivoXML(char *_nome, char *_path) : CDocumento(NULL,NULL)
{
	XML = NULL;
	ArquivoXML = NULL;
	nome = strdup(_nome);
	path = strdup(_path);
	arquivo = (char*) malloc(strlen(nome)+strlen(path)+1);
	sprintf(arquivo,"%s/%s",path,nome);
	ultima_linha = NULL;
	ultima_pagina = NULL;
	linhas = NULL;
	paginas = NULL;
	
}

CArquivoXML::CArquivoXML(char *_arquivo) : CDocumento(NULL,NULL)
{
	ArquivoXML = NULL;
	paginas = ultima_pagina = NULL;
	linhas = ultima_linha = NULL;
	num_paginas = 0;
	num_linhas  = 0;
	
	XML = path = nome = NULL;
	arquivo = (char*)malloc(strlen(_arquivo)+1);
	memcpy(arquivo,_arquivo, strlen(_arquivo));
	arquivo[strlen(_arquivo)] = '\0';
	//arquivo = strdup(_arquivo);
}

CArquivoXML::~CArquivoXML(){
	//printf("Deletando documento %d %p\n", num_paginas, paginas);
	
	while(paginas && num_paginas){
			ultima_pagina = paginas;
			paginas = (CPaginaXML*)paginas->getNext();
			delete ultima_pagina;
			num_paginas--;	
	}
	paginas = NULL;
			
	if(ArquivoXML)	
		delete ArquivoXML;
	
	if(XML)
		free(XML);
		
	if(nome)
		free(nome);
	if(path)
		free(path);
	if(arquivo)
		free(arquivo);
	if(arquivo_xml)
		free(arquivo_xml);
}

TiXmlDocument *CArquivoXML::getArquivoXML(){
	
	if(ArquivoXML)
		return ArquivoXML;

	arquivo_xml = (char*)malloc(strlen(arquivo)+1);
	memcpy(arquivo_xml,arquivo,strlen(arquivo));
	memcpy(&arquivo_xml[strlen(arquivo)-4],".xml\0",5);
	
	ifstream file(arquivo_xml, ifstream::in);
	if(!file.is_open()){
		//Gera arquivo XML
		char pdftohtml[] = "pdftohtml -enc Latin1 -q -p -xml";
		char *comando = new char[strlen(pdftohtml) + strlen(arquivo) + 2];
		sprintf(comando,"%s \"%s\"",pdftohtml,arquivo);
		//printf("%s %s %d\n",comando, arquivo_xml, file.is_open());
		system(comando);
		free(comando);
	}
	file.close();
	file.open(arquivo_xml, ifstream::in);
	
    file.seekg (0, ios::end);
	tamanho = file.tellg();
    XML = (char*) malloc(tamanho+1);
    file.seekg (0, ios::beg);
    file.read(XML, tamanho);
    file.close();
    
    ArquivoXML = new TiXmlDocument (arquivo_xml);
	ArquivoXML->Parse( XML );

	if ( ArquivoXML->Error() )
	{
		printf( "Erro lendo %d %s: %s\n", tamanho, ArquivoXML->Value(), ArquivoXML->ErrorDesc() );
		return NULL;
	}		

	return ArquivoXML;
}		

char *CArquivoXML::getArquivo(){
	return arquivo;
}

char *CArquivoXML::getPath(){
	char *final;
	
	if(path)
		return path;
	
	final = arquivo + strlen(arquivo);
	
	while(arquivo < final){
		final--;
		if(*final == '/'){
			path = (char*)malloc(final - arquivo + 1);
			memcpy(path,arquivo,final - arquivo);
			path[final - arquivo] = '\0';
			return path;
		} 
	}
	return NULL;
}

char *CArquivoXML::getNome(){
	char *inicio;
	
	if(nome)
		return nome;
	
	inicio = arquivo + strlen(arquivo);
	while(arquivo < inicio){
		inicio--;
		if(*inicio == '/'){
			nome = strdup(++inicio);
			return nome;
		} 
	}
	return NULL;
}

///Obtem as paginas XML do arquivo XML 
CPaginaXML *CArquivoXML::getPaginas(){
	//Se houver paginasXML lidas
	if(paginas)
		//Retorna paginasXML lidas
		return (CPaginaXML*)paginas;
	
	num_paginas = 0;
		
	//Adquire primeira pagina da lista de nós XML tag page abaixo da tag pdf2xml	
	TiXmlNode *paginaXML  = getArquivoXML()->RootElement()->FirstChild("page");
	
	CPaginaXML *pagina = NULL;
	CPaginaXML *ultima_pagina = NULL;
	
	//Enquanto houver paginasXML
	while(paginaXML){
		//Cria novo objeto paginaXML com o nó XML, o objeto pai (arquivo), e posicao da pagina
	  	pagina = new CPaginaXML(paginaXML, (CTexto*)this, num_paginas++);
	  	//Se não for a primeira pagina
	  	if(ultima_pagina){
	  		//O proximo da ultima pagina é a pagina atual
	  		pagina->setPrev(*ultima_pagina);
	  		ultima_pagina->setNext(*pagina);
	  	//Se for a primeira pagina
	  	}else
	  		//Primeira pagina da lista igual pagina atual
	  		paginas = pagina;
	  		
	  	//Ultima pagina igual a pagina atual
	  	ultima_pagina = pagina; 	
	  	//Nó xml page igual ao proximo nó XML page
	  	paginaXML = paginaXML->NextSibling("page");
	}
	
  	printf("ArquivoXML %s: %d paginasXML, %d bytes\n", getNome(), num_paginas,sizeof(CPaginaXML)*num_paginas);
  	return paginas;
}	
	//Se não encontrou paginas XML
	/*
	 * if(!num_paginas)
		//Retorna NULL
		return NULL;
	
	//pagina = primeira pagina da lista de objetos CPaginaXML encadeados
	pagina = (CPaginaXML*)paginas;	
	//Aloca bloco de memória de tamanho igual a todos os objetos CPaginaXML
	paginas = (CPaginaXML*)malloc(sizeof(CPaginaXML)*num_paginas);
	//Zera o bloco de memória 
	memset(paginas,0,sizeof(CPaginaXML)*num_paginas);
	
	//Enquanto houver paginas
	while(pagina){
		//printf("\n%d %p %d %p\n", pagina->getPosicao(),pagina->getTexto(),pagina->getTamanho(),(char*)&pagina->getNext());
		//Pega posicao da pagina para posicionar o vetor
		count = pagina->getPosicao();
		//Copia objeto paginaXML previamente criado a partir dos nós XML
		paginas[count] = *pagina;
		//Guarda ponteiro da ultima pagina da lista préviamente alocada
		ultima_pagina = pagina;
		//PaginaXML = proxima paginaXML
		pagina = (CPaginaXML*)pagina->getNext();
		//Apaga Objeto CPaginaXML recem copiado
		delete ultima_pagina;
		
		//Se o objeto não for o ultimo do vetor
		if(count < num_paginas - 1)
			//Proximo objeto CPaginaXML é igual ao objeto posterior no vetor
			paginas[count].setNext(paginas[count+1]);
		else
			//Proximo objeto não existe
			paginas[count].setNext();
			
		//Se o objeto não for o primeiro do vetor
		if(count > 0)
			//Objeto anterior CPaginaXML é igual ao objeto anterior no vetor
			paginas[count].setPrev(paginas[count-1]);
		else
			//Objeto anterior não existe
			paginas[count].setPrev();
	
		//printf("%d %d %p %p %p \n", count, num_paginas, paginas[count].getPrev(), &paginas[count] , paginas[count].getNext());
	}
	//Retorna paginasXML encontradas
	return (CPaginaXML*)paginas;
}
*/
///Adquiri as linhas do Arquivo separadas de forma encadeada
CLinhaXML *CArquivoXML::getLinhas(){
	//Se houver linhas lidas
	if(linhas)
		//Retorna linhas lidas
		return linhas;
	
	//Se não houver texto 
	if(!getTexto() || !getPaginas())
		//retorna NULL
		return NULL;
	
	///Pega paginas
	CPaginaXML *pagina = paginas;
	CLinhaXML *linha;
	int count;
	count = 0;
	
	///Enquanto houver paginas
	while(pagina){
		///Pega lista de linhas da pagina
		linha = pagina->getLinhas();
		///Se houver linhas
		if(linha){
			///Soma o numero de linhas da pagina com o numero total de linhas do documento	
			num_linhas += pagina->num_linhas;
			//printf("%d linhas lidas pagina %d\n", num_palavras, pagina->posicao);
			
			///Se houver uma ultima lista de linhas
			if(ultima_linha){
				///Lista de linhas anterior = ultima lista
				linha->setPrev(*ultima_linha);
				///Proxima lista da ultima lista de linhas = lista de linhas atual
				ultima_linha->setNext(*linha);
			}else
				///inicio da lista total de linhas = primeira lista de palavras da linhas pagina
				linhas = linha;
			
			ultima_linha = pagina->ultima_linha;
			//while((linha = (CLinhaXML*)ultima_linha->getNext()))
				//ultima_linha = palavra;
				
			//ultima_palavra = &palavra[pagina->num_palavras-1];
			//printf("%d %p %p\n", pagina->num_palavras,palavra, ultima_palavra);
		}
		///pagina = proxima pagina
		//count++;
		pagina = (CPaginaXML*)pagina->getNext();
	}
	return linhas;
}

///Adquiri as palavras do Arquivo separadas de forma encadeada
CPalavra *CArquivoXML::getPalavras(){
	//Se houver palavras lidas
	if(palavras)
		//Retorna palavras lidas
		return palavras;
	
	//Se não houver texto 
	if(!getTexto() || !getPaginas())
		//retorna NULL
		return NULL;
	
	///Pega paginas
	CPaginaXML *pagina = (CPaginaXML*)paginas;
	CPalavra *palavra, *ultima_palavra;
	ultima_palavra = NULL;
	int count;
	count = 0;
	
	///Enquanto houver paginas
	while(pagina && count < num_paginas){
		///Pega lista de palavras da pagina
		palavra = pagina->getPalavras();
		///Se houver palavras
		if(palavra){
			///Soma o numero de palavras da pagina com o numero total de palavras do documento	
			num_palavras += pagina->num_palavras;
			//printf("%d palavras lidas pagina %d\n", num_palavras, pagina->posicao);
			
			///Se houver uma ultima lista de palavras
			if(ultima_palavra){
				///Lista de palavras anterior = ultima lista
				palavra->setPrev(*ultima_palavra);
				///Proxima lista da ultima lista de palavras = lista de palavras atual
				ultima_palavra->setNext(*palavra);
			}else
				///inicio da lista total de palavras = primeira lista de palavras da primeira pagina
				palavras = palavra;
			
			ultima_palavra = pagina->ultima_palavra;
			//while((palavra = (CPalavra*)ultima_palavra->getNext()))
				//ultima_palavra = palavra;
			
				
			//ultima_palavra = &palavra[pagina->num_palavras-1];
			//printf("%d %p %p\n", pagina->num_palavras,palavra, ultima_palavra);
		}
		///pagina = proxima pagina
		count++;
		pagina = (CPaginaXML*)pagina->getNext();
	}
	return palavras;
}

///Retorna o texto do arquivo da mesma forma que CDocumento
char *CArquivoXML::getTexto(){
	//Se o texto já estiver formado
	if(texto)
		//Retorna texto
		return texto;
	
	//Se não houverem paginas disponiveis	
	if(!getPaginas())
		//Retorna nulo.
		return NULL;  
	
	CPaginaXML *pagina;
	tamanho = 0;
	//printf("%d\n", paginasXML->getTexto());
	//Calcula tamanho total do documento
	for(pagina = (CPaginaXML*)paginas ; pagina ; pagina = (CPaginaXML*)pagina->getNext() )
		tamanho += pagina->getTamanho();
	
		//printf("%p %x\n", pagina, tamanho);
		
	//Aloca bloco de memória relativo ao tamanho de todas as paginas somadas 
	texto = (char*)malloc(tamanho+1);
	
	int posicao = 0;
	int tamanho_pagina;

	//Para todas as paginas do documento
	for(pagina = (CPaginaXML*)paginas ; pagina ; pagina = (CPaginaXML*)pagina->getNext() ){
		//Pega o tamanho da pagina
		tamanho_pagina = pagina->getTamanho();
		//Atribui o offset em relação ao inicio do documento 
		pagina->setOffset(posicao);
		//Copia o conteudo da pagina para o documento na posição indicada (offset)
		memcpy(texto + posicao,pagina->getTexto(),tamanho_pagina);
		//Desaloca memória da pagina 
		free(pagina->getTexto());
		//Desvincula texto do bloco desalocado
		pagina->setTexto();
		//pagina->setTexto(texto + posicao,tamanho_pagina);
		//Soma o tamanho da pagina para a posição de inicio da próxima pagina
		posicao += tamanho_pagina;
	}
	//printf("%s", texto);
	return texto;
}

char *CArquivoXML::getNormalizado(){
	if(normalizado)
		return normalizado;
		
	if(getTexto())
		return CDocumento::getNormalizado();
	
	return NULL;
}

CArquivoXML &CArquivoXML::Indexar(CPalavra *palavra){
	if(!index)
		index = new CLetra(0);
		
	index->addPalavra(palavra);
	return *this;
}		

CArquivoXML &CArquivoXML::setIndice(CLetra *_index){
	index = _index;
	return *this;
}

CLetra *CArquivoXML::getIndice(){
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
	
