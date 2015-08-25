#include "include/DocumentoIA.h"

/*
 *
 *
typedef struct caracteristicas_t{
	 int		 status;
	 int 		 num_dados;
	 int 		 num_entradas;
	 char 		*descricao[];
	 fann_type **entradas[CARACTERISTICAS_TIPOS];
	 fann_type 	 maximos[CARACTERISTICAS_MAX];
	 fann_type 	 minimos[CARACTERISTICAS_MAX];
	 fann_type 	 diferencas[CARACTERISTICAS_MAX];
} caracteristicas_t;
*/

const char *descricao_linhas[] = {
"tipo da fonte",
"existência de negrito",
"existência de itálico",
"margem superior",
"largura",
"altura",
"posição superior",
"posição esquerda",
"posição inferior",
"posição direita",
"início por numero",
"término por numero"};

CDocumentoIA::CDocumentoIA(char *_arquivo): CArquivoXML(_arquivo){

	sprintf(ident_file,"%s",  _arquivo );
	sprintf(verif_file,"%s",  _arquivo );
	sprintf(&ident_file[strlen(_arquivo)-4], ".ident");
	sprintf(&verif_file[strlen(_arquivo)-4], ".verif");

	caracteristicas_visuais = NULL;
	caracteristicas_espaciais = NULL;
	caracteristicas_estatisticas = NULL;
	caracteristicas_classificatorias = NULL;

	num_caracteristicas_visuais = 3;
	num_caracteristicas_espaciais = 6;
	num_caracteristicas_estatisticas = 38;
	num_caracteristicas_classificatorias = 8;
}

fann_type **CDocumentoIA::getCaracteristicasVisuais(){

	if(caracteristicas_visuais)
		return caracteristicas_visuais;

	if(!getLinhas())
		return NULL;

		int count;
		fann_type *i;
		info_t *info_linha;

		int num_linhas_pagina,
			num_paginas_documento;

		CPaginaXML *pagina = (CPaginaXML*)paginas;
 		CLinhaXML *linha = NULL;

		caracteristicas_visuais = (fann_type**)malloc(num_linhas * sizeof(fann_type*));
		i = (fann_type*)malloc(num_linhas * num_caracteristicas_visuais * sizeof(fann_type));

		count=0;
		num_paginas_documento = num_paginas;
		while(num_paginas_documento--){

			linha = pagina->getLinhas();
			num_linhas_pagina = pagina->num_linhas;

			while(num_linhas_pagina--){
				caracteristicas_visuais[count] = i;

				info_linha = linha->getInfo();

				i[0] = (fann_type)info_linha->negrito?1:0.0;
				i[1] = (fann_type)info_linha->italico?1:0.0;
				i[2] = (fann_type)info_linha->font;

				count++;
				//print_entradas(i,num_caracteristicas_visuais);
				i+= num_caracteristicas_visuais;

				linha =(CLinhaXML*)linha->getNext();
			}
			pagina = (CPaginaXML*)pagina->getNext();
		}
	return caracteristicas_visuais;
}

fann_type **CDocumentoIA::getCaracteristicasEspaciais(){

	if(caracteristicas_espaciais)
		return caracteristicas_espaciais;


	if(!getLinhas())
		return NULL;

	int count;
	int last_top = 0;
	int last_bottom = 0;
	fann_type *i;
	info_t *info_linha,
	*info_pagina;

	int num_linhas_pagina,
		num_paginas_documento;

	CPaginaXML 	*pagina = (CPaginaXML*)paginas;
	CLinhaXML 	*linha 	= NULL;

	caracteristicas_espaciais = (fann_type**)malloc(num_linhas*sizeof(fann_type*));
	i = (fann_type*)malloc(num_linhas*num_caracteristicas_espaciais*sizeof(fann_type));

	count=0;
	info_pagina = pagina->getInfo();

	//Enquanto houver paginas
	num_paginas_documento = num_paginas;
	while(num_paginas_documento--){

		linha = pagina->getLinhas();
		num_linhas_pagina = pagina->num_linhas;

		while(num_linhas_pagina--){
			caracteristicas_espaciais[count] = i;

			info_linha = linha->getInfo();

			if(count && last_top && info_linha->top > last_top)
				i[0] = fabs((fann_type)info_linha->top - ((CLinhaXML*)linha->getPrev())->getInfo()->bottom);
			else
				i[0] = 0;

			if(count && last_bottom && info_linha->bottom > last_bottom)
				i[1] = fabs((fann_type)info_linha->bottom - ((CLinhaXML*)linha->getPrev())->getInfo()->top);
			else
				i[1] = 0;

			last_top = info_linha->top;

			i[2] = (fann_type)info_linha->width;
			i[3] = (fann_type)info_linha->height;
			i[4] = (fann_type)info_linha->top;
			i[5] = (fann_type)info_linha->left;
			//i[5] = (fann_type)info_linha->bottom;
			//i[6] = (fann_type)info_linha->right;
			//i[7] = (fann_type)info_linha->area;
			count++;
			i += num_caracteristicas_espaciais;
			linha =(CLinhaXML*)linha->getNext();
			//print_entradas(i,6);
		}
		pagina = (CPaginaXML*)pagina->getNext();
	}

	return caracteristicas_espaciais;
}

fann_type **CDocumentoIA::getCaracteristicasEstatisticas(){

	if(caracteristicas_estatisticas)
		return caracteristicas_estatisticas;

	if(!getLinhas())
		return NULL;

	int count, u;
	char *t;
	fann_type *i, *frequencia;

	int num_linhas_pagina,
		num_paginas_documento;


	CPaginaXML *pagina = (CPaginaXML*)paginas;
	CLinhaXML  *linha = NULL;
	int			tamanho_linha;
	int			espacos;
	int 	    inicio_letras;
	int 	    inicio_numeros;
	int			num_estatisticas;

	caracteristicas_estatisticas = (fann_type**)malloc(num_linhas*sizeof(fann_type*));
	i  = (fann_type*)malloc(num_linhas*num_caracteristicas_estatisticas*sizeof(fann_type));

	count=0;
	num_estatisticas = 2;

	inicio_numeros = '0';
	inicio_letras = 'A' - 10;
	//Enquanto houver paginas
	num_paginas_documento = num_paginas;
	while(num_paginas_documento--){

		linha = pagina->getLinhas();
		num_linhas_pagina = pagina->num_linhas;

		while(num_linhas_pagina--){

			memset(i,0,num_caracteristicas_estatisticas*sizeof(fann_type));
			caracteristicas_estatisticas[count] = i;

			if(linha->getPalavras()){
				t = linha->getNormalizado();
				espacos = tamanho_linha = 0;
				while(tamanho_linha < num_estatisticas/2 ){

					while(*t==' '){
						t++;
						espacos++;
					}

					if(tamanho_linha + espacos > linha->tamanho)
						break;

					if(t && *t > 32){
						if(*t <= '9' && *t >= '0')
							i[tamanho_linha]= 1;//(fann_type)(*t - inicio_numeros);
						//else
							//i[tamanho_linha]= (fann_type)(*t - inicio_letras);
					}
					//printf("%c", *t);
					t++;
					tamanho_linha++;

				}
				espacos = 0;
				tamanho_linha--;
				t = linha->getNormalizado() + linha->tamanho;
				tamanho_linha = num_estatisticas;
				while(tamanho_linha > num_estatisticas/2){
					tamanho_linha--;
					while(*t==' '){
						t--;
						espacos++;
					}

					if(t && *t > 32){
						if(*t <= '9' && *t >= '0')
							i[tamanho_linha] = 1;//(fann_type)(*t - inicio_numeros);
						//else
							//i[tamanho_linha]= (fann_type)(*t - inicio_letras);
					}
					//printf("%c", *t);
					t--;
					if((num_estatisticas - tamanho_linha + espacos) > linha->tamanho)
						break;
				}


				tamanho_linha = linha->tamanho;
				texto = linha->getNormalizado();
				frequencia = &i[2];
				memset(frequencia,0,36*sizeof(fann_type));
				while(tamanho_linha--){
						t = &texto[tamanho_linha];
						if(*t <= 'Z' && *t >= 'A')
								frequencia[*t - inicio_letras]++;
						else if(*t <= '9' && *t >= '0')
								frequencia[*t - inicio_numeros]++;

				}
				memcpy(&i[2], frequencia, 36*sizeof(fann_type));
			}
				//printf("\n");
			count++;
			i += num_caracteristicas_estatisticas;

			//fwrite(linha->getNormalizado(),1,linha->getTamanho(),stdout);
			//fwrite(t,1,15,stdout);
			//printf("\n");
			//print_entradas(i,num_entradas_estatisticas);
			linha =(CLinhaXML*)linha->getNext();
			//print_entradas(i,4);
		}
		pagina = (CPaginaXML*)pagina->getNext();
	}
	return caracteristicas_estatisticas;
}

fann_type **CDocumentoIA::getCaracteristicasClassificatorias(){
	if(caracteristicas_classificatorias)
		return caracteristicas_classificatorias;

	if(!getLinhas())
		return NULL;

	FILE *ident;
	int num_paginas_documento,
		num_pagina,
		num_linhas_pagina,
		num_linha,
		tipo;

	CPaginaXML *pagina;
	CLinhaXML *linha;

	ident = fopen(ident_file,"r");

	if(ident){

		printf("Lendo caracteristicas classificatorias do arquivo: %s\n", ident_file);

		if(fscanf(ident, "%d\n", &num_paginas_documento) == -1)
			return NULL;

		while(num_paginas_documento--){

			if(fscanf(ident, "%d %d\n", &num_pagina, &num_linhas_pagina) == -1)
				break;

			pagina = getPaginas();
			while(num_pagina-- && pagina)
				pagina = (CPaginaXML*)pagina->getNext();

			while(num_linhas_pagina--){

				if(fscanf(ident, "%d %d\n", &num_linha, &tipo) == -1)
					break;

				if(pagina){

					linha = (CLinhaXML*)pagina->getLinhas();
					while(num_linha-- && linha)
						linha = (CLinhaXML*)linha->getNext();
					if(linha){
						linha->info.tipo = tipo;
						//fwrite(linha->getTexto(),1,linha->getTamanho(),stdout);
						//printf("\n");
					}
				}
			}
		}
		fclose(ident);
	}

	fann_type *i;
	int count, u;
	info_t *info_linha;

	caracteristicas_classificatorias = 	(fann_type**)malloc(num_linhas	*	sizeof(fann_type*));
	i = (fann_type*) malloc(num_linhas	*	num_caracteristicas_classificatorias	*	sizeof(fann_type));

	count = 0;
	pagina = getPaginas();
	num_paginas_documento = num_paginas;
	while(num_paginas_documento--){
		linha = (CLinhaXML*)pagina->getLinhas();
		num_linhas_pagina = pagina->num_linhas;
		while(num_linhas_pagina--){
			caracteristicas_classificatorias[count] = i;

			info_linha = linha->getInfo();
			u = num_caracteristicas_classificatorias;
//			printf("linha %p, %p, %p, %d, %d, %d\n",linha, linha->next, i, u, linha->posicao, count);
		//	while(u--)
			//	i[u] = 0.01;
			memset(i,0,sizeof(fann_type)*num_caracteristicas_classificatorias);
			if(info_linha->tipo > 0)
				i[info_linha->tipo] = 100;
			//i[0] = info_linha->tipo + 1;
			count++;
			//print_entradas(i,num_caracteristicas_classificatorias);
			i += num_caracteristicas_classificatorias;
			linha = (CLinhaXML*)linha->next;

		}
	//	printf("pagina %p, %p, %d\n",pagina, pagina->next, pagina->posicao);
		pagina = (CPaginaXML*)pagina->next;
	}

	return caracteristicas_classificatorias;
}

int CDocumentoIA::reproduzCaracteristicasClassificatorias(fann_type **dados, int num_dados){
	if(!getLinhas() || !dados){

		return NULL;
	}
	FILE *verif;
	int tipo,
		num_linhas_pagina,
		num_paginas_documento,
		i, count, offset;

	CPaginaXML *pagina;
	CLinhaXML *linha;

	fann_type max, *entradas;

	verif = fopen(verif_file,"w");

	if(verif){
		//num_entradas = caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS].num_entradas;
		//entradas = caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS].entradas;
		printf("Reproduzindo %d caracteristicas classificatórias identificadas... \n", num_dados);
		count = num_dados - 1;
		//offset = num_caracteristicas_visuais + num_caracteristicas_espaciais + num_caracteristicas_estatisticas;

		pagina = getPaginas();
		fprintf(verif, "%d\n", num_paginas);
		num_paginas_documento = num_paginas;
		while(num_paginas_documento--){
			linha = (CLinhaXML*)pagina->getLinhas();
			num_linhas_pagina = pagina->num_linhas;
			fprintf(verif, "%d %d\n", pagina->posicao, num_linhas_pagina);
			while(num_linhas_pagina--){
				tipo = 0;
				max = 0;
				if(!count)
					break;
				//i = offset + num_caracteristicas_classificatorias;
				i= num_caracteristicas_classificatorias;
				entradas = dados[count];
				//printf("%d %d %d %d\n",tipo,max, i, num_caracteristicas, count);
				//print_entradas(entradas, i);
				while(i--){
					if(entradas[i] > max){
						//printf( "%d %d, %d %f %f\n", linha->posicao, tipo, i, max,entradas[i] );
						max = entradas[i];
						tipo = i;

					}
				}

				if(entradas[tipo] < 50)
					tipo = -1;
				else if(entradas[tipo] < 90)
					tipo = 0;


				//if(tipo){
				printf( "%d %d %f => ", linha->posicao, tipo, max);
				print_entradas(entradas, num_caracteristicas_classificatorias);
				linha->info.tipo = tipo;
				fprintf(verif, "%d %d\n", linha->posicao, tipo);
				//}
				linha = (CLinhaXML*)linha->next;
				count--;
			}
			pagina = (CPaginaXML*)pagina->next;
		}
		fclose(verif);
		return 1;
	}
	return 0;
}
