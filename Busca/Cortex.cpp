/*
 * Cortex.cpp
 *
 *  Created on: 13/06/2010
 *      Author: ivan
 */
#include "include/Cortex.h"
#include "include/Caracteristicas.h"

CCortex::CCortex(CCerebro *_cerebro){
	cerebro = _cerebro;

	memset(dados,0,sizeof(training_data*)*CARACTERISTICAS_GRUPOS);
	memset(lobos,0,sizeof(CLobo*)*CARACTERISTICAS_GRUPOS);
	memset(caracteristicas,0,sizeof(caracteristicas_t*)*CARACTERISTICAS_GRUPOS);

	num_dados = 0;
	caracteristicas_total = 0;
}

CLobo *CCortex::getLobo(int grupo){
	if(lobos[grupo])
		return lobos[grupo];

	if(!getDados(grupo)){
		printf("Sem entradas no grupo %d\n", grupo);
		return NULL;
	}
	lobos[grupo] = new CLobo(this, dados[grupo], grupo);
	return lobos[grupo];
}

CLobo *CCortex::getLoboVisual(){
	return getLobo(CARACTERISTICAS_VISUAIS);
}

CLobo *CCortex::getLoboEspacial(){
	return getLobo(CARACTERISTICAS_ESPACIAIS);
}

CLobo *CCortex::getLoboEstatistico(){
	return getLobo(CARACTERISTICAS_ESTATISTICAS);
}

CLobo *CCortex::getLoboClassificatorio(){
	return getLobo(CARACTERISTICAS_CLASSIFICATORIAS);
}

training_data *CCortex::getDadosVisuais(){
	return getDados(CARACTERISTICAS_VISUAIS);
}

training_data *CCortex::getDadosEspaciais(){
	return getDados(CARACTERISTICAS_ESPACIAIS);
}

training_data *CCortex::getDadosEstatisticos(){
	return getDados(CARACTERISTICAS_ESTATISTICAS);
}

training_data *CCortex::getDadosClassificatorios(){
	return getDados(CARACTERISTICAS_CLASSIFICATORIAS);
}


training_data *CCortex::getDadosOld(int grupo){

	if(dados[grupo])
		return dados[grupo];

	// Se nao houver caracteristias, nao houver caracteristicas visuais, ou só houver caracteristicas visuais
	if( !getCaracteristicas() ||
		caracteristicas_total < 2 ||
		!caracteristicas[grupo]->entradas[CARACTERISTICAS_ORIGINAIS])
		return NULL;

	/*DESCONHECIDAS
	 * Unificar as caracteristicas paralelas de forma a generalizar as caracteristicas visuais
	 */
	int i,u,count,num_entradas_total;

	fann_type *in_src, *in_dst,*out_src, *out_dst, **in, **out, **input;
	fann_type *diferencas, *diferenca;
	fann_type **entradas;
	int num_entradas;

	//memset(entradas,0,sizeof(fann_type*)*CARACTERISTICAS_GRUPOS);

	// Contar total de entradas para criar a lista de caracteristicas
	count = 0;
	printf("Entradas do grupo %d:", grupo);

		entradas = caracteristicas[grupo]->entradas[CARACTERISTICAS_ORIGINAIS];
		num_entradas = caracteristicas[grupo]->num_entradas;
		diferencas = caracteristicas[grupo]->diferencas;


	//printf("%p\n", caracteristicas[grupo][0]);
	printf("Grupo %d: %d dados com %d entradas e %d saidas\n", grupo, num_dados, num_entradas, num_entradas);
	in = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	out = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	in_dst = (fann_type*)malloc( num_entradas * num_dados * sizeof(fann_type) );
	out_dst = (fann_type*)malloc( num_entradas * num_dados * sizeof(fann_type) );
	while(count < num_dados	){
		//in_dst  = ( fann_type * )( input  + ( count * num_entradas_total));
		//out_dst = ( fann_type * )( output + ( count * num_entradas[grupo]));
		in[count]  = in_dst;
		out[count] = out_dst;
		in_src = entradas[count];

		memcpy(	 in_dst , in_src , sizeof(fann_type) * num_entradas );

		u = num_entradas;
		while(u--)
			in_dst[u] /= diferencas[u];

		in_dst += num_entradas;

		out_src = entradas[count];
		memcpy(out_dst , out_src , sizeof(fann_type) * num_entradas );

		u = num_entradas;
		while(u--)
			out_dst[u] /= diferencas[u];

		out_dst += num_entradas;


		//in_dst += num_entradas_total;
		//out_dst += num_entradas[grupo];
		//print_entradas(in_dst - num_entradas_total, num_entradas_total);
		//print_entradas(out_dst - num_entradas[grupo], num_entradas[grupo]);
		count++;
	}

	dados[grupo] = new training_data();
	dados[grupo]->set_train_data(num_dados, num_entradas, in, num_entradas, out);

	free(in[0]);
	free(out[0]);
	free(in);
	free(out);

	return dados[grupo];
}


training_data *CCortex::getDados(int grupo){

	if(dados[grupo])
		return dados[grupo];

	// Se nao houver caracteristias, nao houver caracteristicas visuais, ou só houver caracteristicas visuais
	if( !getCaracteristicas() ||
		caracteristicas_total < 2 ||
		!caracteristicas[grupo]->entradas[CARACTERISTICAS_ORIGINAIS])
		return NULL;

	/*DESCONHECIDAS
	 * Unificar as caracteristicas paralelas de forma a generalizar as caracteristicas visuais
	 */
	int i,u,z,count,num_entradas_total, num_entradas_grupo;

	fann_type *in_src, *in_dst,*out_src, *out_dst, **in, **out, **input;
	fann_type *diferencas[CARACTERISTICAS_GRUPOS], *diferenca;
	fann_type *maximos[CARACTERISTICAS_GRUPOS], *maximo;
	fann_type *minimos[CARACTERISTICAS_GRUPOS], *minimo;
	fann_type **entradas[CARACTERISTICAS_GRUPOS], **e;
	int num_entradas[CARACTERISTICAS_GRUPOS];

	//memset(entradas,0,sizeof(fann_type*)*CARACTERISTICAS_GRUPOS);

	// Contar total de entradas para criar a lista de caracteristicas
	count = num_entradas_total = 0;
	printf("Entradas do grupo %d:", grupo);
	for( i = 0 ; i < CARACTERISTICAS_GRUPOS ; i++ ){
			e = entradas[i] = caracteristicas[i]->entradas[CARACTERISTICAS_ORIGINAIS];
			num_entradas_grupo = num_entradas[i] = caracteristicas[i]->num_entradas;
			diferenca = diferencas[i] = caracteristicas[i]->diferencas;
			maximo = maximos[i] = caracteristicas[i]->maximos;
			minimo = minimos[i] = caracteristicas[i]->minimos;

			if(!memcmp(maximo,zero,num_entradas_grupo * sizeof(fann_type))){

				u=num_entradas_grupo;
				while(u--){
					maximo[u] = 0;
					minimo[u] = 999999999;
				}

				count=num_dados;
				while(count--){
					u=num_entradas_grupo;
					in_src = e[count];
					while(u--){
						if(in_src[u] > maximo[u])
							maximo[u] = in_src[u];
						else if(in_src[u] < minimo[u])
							minimo[u] = in_src[u];
					}
				}
				u = num_entradas_grupo;

				while(u--)
					diferenca[u] = maximo[u] - minimo[u];

				printf("\nMáximos:\t");
				print_entradas(maximo,num_entradas_grupo);
				printf("Mínimos:\t");
				print_entradas(minimo,num_entradas_grupo);
				printf("Diferenças:\t");
				print_entradas(diferenca,num_entradas_grupo);
			}

			if(i != grupo && num_entradas_grupo){
				num_entradas_total += num_entradas_grupo;
				printf(" %d", i);
			}

		//printf(" %p ", entradas[i]);
	}
	printf("\n");


	//printf("%p\n", caracteristicas[grupo][0]);
	printf("Grupo %d: %d dados com %d entradas e %d saidas\n", grupo, num_dados, num_entradas_total, num_entradas[grupo]);
	in = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	out = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	in_dst = (fann_type*)malloc( num_entradas_total * num_dados * sizeof(fann_type) );
	out_dst = (fann_type*)malloc( num_entradas[grupo] * num_dados * sizeof(fann_type) );

	count = 0;
	while(count < num_dados	){
		//in_dst  = ( fann_type * )( input  + ( count * num_entradas_total));
		//out_dst = ( fann_type * )( output + ( count * num_entradas[grupo]));
		in[count]  = in_dst;
		out[count] = out_dst;
		for(i = 0; i < CARACTERISTICAS_GRUPOS ; i++ ){
			num_entradas_grupo = num_entradas[i];
			if(num_entradas_grupo){
				diferenca = diferencas[i];
				maximo = maximos[i];
				minimo = minimos[i];
				e = entradas[i];

				if( i != grupo){
					in_src = e[count];
					memcpy(	 in_dst , in_src , sizeof(fann_type) * num_entradas_grupo );

					u = num_entradas_grupo;
					while(u--)
						if(diferenca[u] && diferenca[u] != 1){
							//in_dst[u] /= diferenca[u];
							//in_dst[u] = ((in_dst[u] / diferenca[u])*2) - 1;
							in_dst[u] = (in_dst[u] - minimo[u]) / diferenca[u];
							//if(in_dst[u] > 1){
								//print_entradas(in_src,num_entradas_grupo);
								//print_entradas(in_dst,num_entradas_grupo);
							//}
						}
					in_dst += num_entradas_grupo;

				}else{
					out_src = e[count];
					memcpy(out_dst , out_src , sizeof(fann_type) * num_entradas_grupo );

					u = num_entradas_grupo;
					while(u--)
						if(diferenca[u] && diferenca[u] != 1)
							//out_dst[u] /= diferenca[u];
							//out_dst[u] = ((out_dst[u] / diferenca[u])*2) - 1;
							out_dst[u] = (out_dst[u] - minimo[u]) / diferenca[u];

					out_dst += num_entradas_grupo;

				}
			}
		}
		//in_dst += num_entradas_total;
		//out_dst += num_entradas[grupo];
		//print_entradas(in_dst - num_entradas_total, num_entradas_total);
		//print_entradas(out_dst - num_entradas[grupo], num_entradas[grupo]);
		count++;
	}

	dados[grupo] = new training_data();
	dados[grupo]->set_train_data(num_dados, num_entradas_total, in, num_entradas[grupo], out);

	free(in[0]);
	free(out[0]);
	free(in);
	free(out);

	return dados[grupo];
}

caracteristicas_t **CCortex::getCaracteristicas(){
	if(num_dados)
		return caracteristicas;

	if(!cerebro || !cerebro->getNumAmostras()){
		printf("Sem amostras disponiveis\n");
		return NULL;
	}

	caracteristicas[CARACTERISTICAS_VISUAIS] = cerebro->getCaracteristicasVisuais();

	caracteristicas[CARACTERISTICAS_ESPACIAIS] = cerebro->getCaracteristicasEspaciais();

	caracteristicas[CARACTERISTICAS_ESTATISTICAS] = cerebro->getCaracteristicasEstatisticas();

	caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS] = cerebro->getCaracteristicasClassificatorias();

	int i,u;

	for( i=u=0 ; i < CARACTERISTICAS_GRUPOS ; i++ ){

		if(caracteristicas[i] && caracteristicas[i]->entradas[CARACTERISTICAS_ORIGINAIS])
			u++;
		//else
		// caracteristicas[i] = NULL;
	}
		caracteristicas_total = u;

	if(!u)
		return NULL;

	num_dados = cerebro->getNumAmostras();

	printf("%d amostras lidas, %d grupos de entradas, %d entradas visuais, %d entradas espaciais, %d entradas estatisticas e %d entradas classificatorias\n",
			num_dados,
			caracteristicas_total,
			caracteristicas[CARACTERISTICAS_VISUAIS]->num_entradas,
			caracteristicas[CARACTERISTICAS_ESPACIAIS]->num_entradas,
			caracteristicas[CARACTERISTICAS_ESTATISTICAS]->num_entradas,
			caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS]->num_entradas);

	return caracteristicas;
}

int CCortex::deduzCaracteristicas(int grupo){

	// Se nao houver caracteristias, nao houver caracteristicas visuais, ou só houver caracteristicas visuais
	if( !getCaracteristicas() ||
		!generalizaCaracteristicas() ||
		 caracteristicas_total < 2 ||
		!caracteristicas[grupo]->entradas )
		return NULL;

	int i,u,count,num_entradas_total, num_saidas_total;

	fann_type *in_src, *in_dst,*out_src, *out_dst, **in, **out;
	fann_type **entradas_generalizadas[CARACTERISTICAS_GRUPOS];
	int num_entradas[CARACTERISTICAS_GRUPOS];

	// Contar total de entradas para criar a lista de caracteristicas
	count = num_entradas_total = 0;
	printf("Entradas do grupo generalizado %d:", grupo);
	for( i = 0 ; i < CARACTERISTICAS_GRUPOS ; i++ ){
		if(i != grupo){
			if(caracteristicas[i]->entradas[CARACTERISTICAS_GENERALIZADAS]){
			num_entradas_total += caracteristicas[i]->num_entradas;
			printf(" %d", i);
			}
			entradas_generalizadas[i] = caracteristicas[i]->entradas[CARACTERISTICAS_GENERALIZADAS];
		}
		num_entradas[i] = caracteristicas[i]->num_entradas;
		//printf(" %p ", entradas[i]);
	}
	printf("\n");

	if(caracteristicas[grupo]->entradas[CARACTERISTICAS_EXTERNAS]){
		printf("Deduzindo a partir de caracteristicas externas.\n");
		entradas_generalizadas[grupo] = caracteristicas[grupo]->entradas[CARACTERISTICAS_EXTERNAS];
	}else{
		entradas_generalizadas[grupo] = caracteristicas[grupo]->entradas[CARACTERISTICAS_GENERALIZADAS];
	}


	num_saidas_total = caracteristicas[grupo]->num_entradas;

	//printf("%p\n", caracteristicas[grupo][0]);
	printf("Grupo %d: %d dados generalizados com %d entradas e %d saidas\n", grupo, num_dados, num_entradas_total, num_entradas[grupo]);
	in = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	in_dst = (fann_type*)malloc(num_dados * num_entradas_total * sizeof(fann_type) );
	//out = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	//out_dst = (fann_type*)malloc(num_dados * num_saidas_total * sizeof(fann_type) );

	count = num_dados;
	while(count--){
		in[count]  = in_dst;
	//	out[count] = out_dst;
		for(i = 0; i < CARACTERISTICAS_GRUPOS ; i++ ){
			if( i != grupo ){
				if( entradas_generalizadas[i] ){
					in_src = entradas_generalizadas[i][count];
					memcpy(	in_dst , in_src , sizeof(fann_type) * num_entradas[i] );
					in_dst += num_entradas[i];
				}
			}
//			else{
//				out_src = entradas_generalizadas[i][count];
//				memcpy(out_dst , out_src , sizeof(fann_type) * num_entradas[i] );
//				out_dst += num_entradas[i];
//			}
		}
		//print_entradas(in_dst  - num_entradas_total, num_entradas_total);
		//print_entradas(out_dst - num_saidas_total  , num_saidas_total);
	}

	out = (fann_type**) malloc( num_dados * num_saidas_total * sizeof(fann_type) );
	memcpy(out, entradas_generalizadas[grupo], num_dados * num_saidas_total * sizeof(fann_type) );
	printf("Out 1 %p\n", out);
	CCaracteristicas *entradas,*entradas_comparadas,*tmp;
	int num_entradas_unicas = 0;
	count = num_dados;
	entradas = NULL;
	//out = entradas_generalizadas[grupo];
	while(count--){
		entradas_comparadas = new CCaracteristicas(num_entradas_total, in[count], num_saidas_total, out[count]);
		if(entradas){
			for(tmp = entradas ; tmp ; tmp = tmp->next)
				if(tmp->Compara(entradas_comparadas))
					break;

			if(!tmp){
				entradas_comparadas->next = entradas;
				entradas = entradas_comparadas;
				num_entradas_unicas++;
			}

		}else{
			entradas = entradas_comparadas;
			num_entradas_unicas++;
		}
	}
	for(tmp = entradas ; tmp ; tmp = tmp->next){
		//tmp->Normaliza();
		tmp->Replica();
	}

	caracteristicas[grupo]->entradas[CARACTERISTICAS_DEDUZIDAS] = out;
	printf("%d padrões encontrados.\n", num_entradas_unicas);

	free(in[num_dados-1]);
	//free(out[num_dados-1]);
	free(in);
	//free(out);

	return 1;
}

int CCortex::generalizaCaracteristicas(){
/*
 * getRespostas do lobo contem as respostas generalizadas
 * elas devem ser multiplicadas e arredondadas.
 *
 * */
	int i,u,count,num_entradas;
	fann_type **caracteristicas_generalizadas, **respostas,*resposta, *caracteristica_generalizada, *diferenca,*minimo;

	for(i=0;i<CARACTERISTICAS_GRUPOS;i++){
		if(getLobo(i) && !caracteristicas[i]->entradas[CARACTERISTICAS_GENERALIZADAS]){
			lobos[i]->generalizaDados();
			respostas = lobos[i]->getRespostas();
			num_entradas  = caracteristicas[i]->num_entradas;
			diferenca = caracteristicas[i]->diferencas;
			minimo = caracteristicas[i]->minimos;

			caracteristicas_generalizadas = (fann_type**)malloc( num_dados * sizeof(fann_type) );
			caracteristica_generalizada   = (fann_type*) malloc( num_dados * num_entradas * sizeof(fann_type) );

			//caracteristicas_generalizadas = caracteristicas[i]->entradas[CARACTERISTICAS_ORIGINAIS];
			//caracteristica_generalizada = caracteristicas_generalizadas[0];

			count = 0;
			while(count < num_dados){
				caracteristicas_generalizadas[count] = caracteristica_generalizada;
				resposta = respostas[count];
				u=num_entradas;
				memcpy(caracteristica_generalizada,resposta,num_entradas*sizeof(fann_type));
					while(u--)
					caracteristica_generalizada[u] = (int)round((caracteristica_generalizada[u] * diferenca[u]) + minimo[u]);
					//caracteristica_generalizada[u] = (int)round((((caracteristica_generalizada[u]+1)/2)*diferenca[u]));
					//caracteristica_generalizada[u] = (int)round(caracteristica_generalizada[u]*diferenca[u]);
				//print_entradas(caracteristica_generalizada, num_entradas);
				caracteristica_generalizada += num_entradas;
				count++;
			}
			caracteristicas[i]->status = CARACTERISTICAS_GENERALIZADAS;
			caracteristicas[i]->entradas[CARACTERISTICAS_GENERALIZADAS] = caracteristicas_generalizadas;
		}
	}

	return 1;
}

int CCortex::analisaCaracteristicas(){

	if(!getCaracteristicas() || caracteristicas_total < 2)
		return NULL;

	if(getLoboVisual()){
		lobos[CARACTERISTICAS_VISUAIS]->analisaDados();
		caracteristicas[CARACTERISTICAS_VISUAIS]->entradas[CARACTERISTICAS_ANALISADAS] =
		caracteristicas[CARACTERISTICAS_VISUAIS]->entradas[CARACTERISTICAS_ORIGINAIS];
	}if(getLoboEspacial()){
		lobos[CARACTERISTICAS_ESPACIAIS]->analisaDados();
		caracteristicas[CARACTERISTICAS_ESPACIAIS]->entradas[CARACTERISTICAS_ANALISADAS] =
		caracteristicas[CARACTERISTICAS_ESPACIAIS]->entradas[CARACTERISTICAS_ORIGINAIS];
	}if(getLoboEstatistico()){
		lobos[CARACTERISTICAS_ESTATISTICAS]->analisaDados();
		caracteristicas[CARACTERISTICAS_ESTATISTICAS]->entradas[CARACTERISTICAS_ANALISADAS] =
		caracteristicas[CARACTERISTICAS_ESTATISTICAS]->entradas[CARACTERISTICAS_ORIGINAIS];
	}if(getLoboClassificatorio()){
		lobos[CARACTERISTICAS_CLASSIFICATORIAS]->analisaDados();
		caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS]->entradas[CARACTERISTICAS_ANALISADAS] =
		caracteristicas[CARACTERISTICAS_CLASSIFICATORIAS]->entradas[CARACTERISTICAS_ORIGINAIS];
	}
	return 1;
}

CCerebro *CCortex::getCerebro(){
	return cerebro;
}
