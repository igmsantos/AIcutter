/*
 * Lobo.cpp
 *
 *  Created on: 12/06/2010
 *      Author: ivan
 */
#include "include/Lobo.h"
#include "include/Caracteristicas.h"
#define LOOPS 10
#define BIT_FAIL_COUNTER 100
#define MSE_COUNTER 100

#include <stdio.h>

#define RESET		0
#define BRIGHT 		1
#define DIM			2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	GRAY		7
#define	WHITE		8

#define CLEAR_SCREEN "\033[2J"
/*int main()
{	textcolor(BRIGHT, RED, BLACK);
	printf("In color\n");
	textcolor(RESET, WHITE, BLACK);
	return 0;
}*/


void clear_screen(){
	//     clear();
	  //   refresh();
	system("clear");
}
void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

void textcolor_str(char *buffer, int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(buffer, "%c[%d;%d;%dm\0", 0x1B, attr, fg + 30, bg + 40);
}

void cor_nivel(int nivel, char *buffer){
	char color_buffer[16];
	char default_buffer[16];

	switch((int)round((float)(nivel/10))){
	case 9:
		textcolor_str(color_buffer,BRIGHT, WHITE, BLACK);
		break;
	case 8:
		textcolor_str(color_buffer,BRIGHT, WHITE, BLACK);
		break;
	case 7:
		textcolor_str(color_buffer,BRIGHT, YELLOW, BLACK);
		break;
	case 6:
		textcolor_str(color_buffer,BRIGHT, GREEN, BLACK);
		break;
	case 5:
		textcolor_str(color_buffer,BRIGHT, CYAN, BLACK);
		break;
	case 4:
		textcolor_str(color_buffer,BRIGHT, BLUE, BLACK);
		break;
	case 3:
		textcolor_str(color_buffer,BRIGHT, MAGENTA, BLACK);
		break;
	case 2:
		textcolor_str(color_buffer,BRIGHT, RED, BLACK);
		break;
	case 10:
		textcolor_str(color_buffer,BLINK, GRAY, BLACK);
		break;
	default:
		textcolor_str(color_buffer,RESET, GRAY, BLACK);
		break;
	}

	textcolor_str(default_buffer,RESET, WHITE, BLACK);
	sprintf(buffer,"%s%d%s", color_buffer,nivel,default_buffer);

}

int callback_html(FANN::neural_net &rede_neural,
	 training_data &dados,
	 unsigned int turnos_max,
	 unsigned int turnos_por_amostra,
	 float margem_de_erro,
	 unsigned int turnos,
	 void *_lobo){

	 static FILE *plot = NULL;

 	 static float total = 0;
 	 static float last_MSE = 0;
 	 static float last_bit_fail = 0;

	 static int loops = 0;
	 static int MSE_counter = 0;
	 static int bit_fail_counter = 0;
	 static float steepness = 0;
	 static float aprendizagem = 0;
	 static float last_aprendizagem = 0;

	 static bool set_steepness = false;
	 static bool set_aprendizagem = false;

	 char buffer[20000];
	 char MSE_buffer[64];
	 char bit_fail_buffer[64];
	 float diferenca_MSE;
	 float MSE;
	 int bit_fail;

	MSE = rede_neural.get_MSE();
	bit_fail = rede_neural.get_bit_fail();

	//printf("%f %f %d\n", MSE, last_MSE, (MSE - last_MSE) > 0.000001);
	if(last_MSE)
		diferenca_MSE = (last_MSE - MSE);
	else
		diferenca_MSE = 0;

	if(last_bit_fail == bit_fail){
		if(bit_fail_counter < BIT_FAIL_COUNTER)
		bit_fail_counter++;
	}else if(bit_fail_counter > 0)
		bit_fail_counter--;// = bit_fail_counter * 0.85;

	if(fabs(diferenca_MSE) < margem_de_erro){
		if(MSE_counter < MSE_COUNTER)
			MSE_counter++;
	}else if(MSE_counter > 0)
			MSE_counter--;// = MSE_counter * 0.9;

	CLobo *lobo = (CLobo*)_lobo;

	//printf("%d\n", bit_fail_counter);
	//printf("%f %f\n",last_MSE, diferenca_MSE);
	if(last_MSE){
		fprintf(plot, "%d %d %d %d\n",turnos, (int)round(diferenca_MSE), bit_fail_counter, MSE_counter);
		fflush(plot);
		total = total + diferenca_MSE;
	}

	last_MSE = MSE;
	last_bit_fail = bit_fail;

	if(set_aprendizagem){

		aprendizagem = (1-((float)(MSE_counter + bit_fail_counter) / 400));
		if(!last_aprendizagem)
			last_aprendizagem = aprendizagem;

		if(last_aprendizagem > aprendizagem && !((MSE_counter + bit_fail_counter)%2)){
			rede_neural.set_learning_rate(aprendizagem);
			last_aprendizagem = aprendizagem;
		}
	}


	if(set_steepness){
		if(!steepness)
			steepness = rede_neural.get_activation_steepness(1,0);

		rede_neural.set_activation_steepness_layer(
			(1-((float)(MSE_counter + bit_fail_counter) / 100))*steepness
			,1);
			//set_sneepness = true;
	}

	loops++;
	if(turnos > 1){
		if(loops < LOOPS && (bit_fail_counter < BIT_FAIL_COUNTER || MSE_counter < MSE_COUNTER) )
			return 0;
	}else{
		bit_fail_counter = 0;
		MSE_counter = 0;
		last_aprendizagem = 0;
		if(plot)
			fclose(plot);

		plot = fopen(lobo->plot_file, "w");
	}
	loops = 0;

	memset(buffer,0,20000);

	//clear_screen();

	cor_nivel(bit_fail_counter,bit_fail_buffer);
    cor_nivel(MSE_counter,MSE_buffer);

	//printf("%f\n",round((float)bit_fail_counter/10));
	sprintf((char*)(buffer + strlen(buffer)), "%08d turnos, precisão %1.6f(%-1.6f)(%s%), nivel de adaptacao %d(%s%) \0 ",
											  turnos, MSE, diferenca_MSE, MSE_buffer, bit_fail, bit_fail_buffer);
	sprintf((char*)(buffer + strlen(buffer)),", depuracao %-1.6f, aprendizagem %f, passo %f\0", total,last_aprendizagem,steepness);
	sprintf((char*)(buffer + strlen(buffer)),"\n\0");
	printf(buffer);
	//textcolor(RESET, WHITE, BLACK);
	//rede_neural.print_connections(buffer);
	//printf(buffer);


	if(bit_fail_counter == BIT_FAIL_COUNTER && MSE_counter == MSE_COUNTER){
			printf("Treino completado em %08d, adaptacao %f, MSE %f\n",turnos, bit_fail, MSE);
			return -1;
		}

	return 0;
}

CLobo::CLobo(CCortex *_cortex, training_data *_dados, int _grupo){

	cortex = _cortex;
	grupo = _grupo;
	//Parametros do treinamento
	turnos = 100000;
	turnos_por_amostra = 10;
	margem_de_erro 	= 0.0000005;//0.00000001; //0.000000000121076
	aprendizagem 	= 0.7;
	conectividade = 1;

	dados = _dados;
	//Parametros da rede neural
	num_caracteristicas = dados->num_input_train_data();
	num_respostas = dados->num_output_train_data();
	num_neuronios_invisiveis = 5;//ceil( ((num_caracteristicas + num_respostas)/4) * cortex->caracteristicas[grupo]->complexibilidade);
	num_neuronios_invisiveis2 = 4; //round( (num_caracteristicas + num_respostas) / 5 );// * cortex->caracteristicas[grupo]->complexibilidade);
	num_camadas = 3;
	rede_neural = NULL;

	camadas = (unsigned int *)malloc(sizeof(unsigned int)*num_camadas);
	camadas[0] = num_caracteristicas;
	camadas[1] = num_neuronios_invisiveis;
	//camadas[2] = num_neuronios_invisiveis2;
	camadas[2] = num_respostas;

	char *documento = cortex->getCerebro()->getDocumento()->arquivo_xml;

	sprintf(ann_file,"%s\0", documento );
	sprintf(train_file,"%s\0",  documento );
	sprintf(plot_file,"%s\0",  documento );
	sprintf(map_file,"%s\0",  documento );

	sprintf(&ann_file[strlen(documento)-4]	, ".%d.net\0", grupo);
	sprintf(&train_file[strlen(documento)-4], ".%d.train\0", grupo);
	sprintf(&plot_file[strlen(documento)-4]	, ".%d.plot\0", grupo);
	sprintf(&map_file[strlen(documento)-4], ".%d.map\0", grupo);

	printf("Criando lobo para o grupo %d: ", grupo);
	printf("%d caracteristicas, %d neuronios invisiveis, %d respostas, %d%% conectados. Complexibilidade %f.\n",
			num_caracteristicas, num_neuronios_invisiveis, num_respostas, (int)round(100 * conectividade),
			cortex->caracteristicas[grupo]->complexibilidade);

	dados->save_train(train_file);

}

neural_net *CLobo::getRedeNeural(){
	if(rede_neural)
		return rede_neural;

	if(!getCaracteristicas() || !getRespostas())
		return NULL;

	rede_neural = new neural_net();
	if(!rede_neural->create_from_file(ann_file)){
	//if(1){
		rede_neural->create_sparse_array(conectividade,num_camadas,camadas);
		//rede_neural->create_shortcut_array(num_camadas,camadas);

		//rede_neural->create_sparse(0.8, 4, num_input, 5, 5, num_output);
		//rede_neural->create_sparse(0.8, 3, num_input, 10, num_output);
		//rede_neural->create_sparse(0.95, 2, num_input, num_output);

		//rede_neural->create_shortcut(4, num_input, 5, 5, num_output);
		//rede_neural->create_shortcut(3, num_input, 10, num_output);
		//rede_neural->create_shortcut(2, num_input, num_output);


		//ALGORITMO DE TREINAMENTO
		//rede_neural->set_training_algorithm(TRAIN_INCREMENTAL);
		//rede_neural->set_training_algorithm(TRAIN_BATCH);
		//rede_neural->set_training_algorithm(TRAIN_QUICKPROP);
		rede_neural->set_training_algorithm(TRAIN_RPROP);

		//------ALGORITMO DE ATIVACAO LAYER1
		//rede_neural->set_activation_function_layer(LINEAR,1);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE,1);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE_SYMMETRIC,1);
		//rede_neural->set_activation_function_layer(SIGMOID,1);
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC,1);
		//rede_neural->set_activation_function_layer(SIGMOID_STEPWISE,1);
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC_STEPWISE,1);
		//rede_neural->set_activation_function_layer(ELLIOT,1);
		//rede_neural->set_activation_function_layer(ELLIOT_SYMMETRIC,1);
		rede_neural->set_activation_function_layer(GAUSSIAN,1);
		//rede_neural->set_activation_function_layer(GAUSSIAN_SYMMETRIC,1);
		//rede_neural->set_activation_function_layer(COS_SYMMETRIC,1);
		//rede_neural->set_activation_function_layer(SIN_SYMMETRIC,1);

		//rede_neural->set_activation_function(SIGMOID_SYMMETRIC,1,0);
		//rede_neural->set_activation_function(SIGMOID_SYMMETRIC,1,1);

		//------ALGORITMO DE ATIVACAO LAYER2
		//rede_neural->set_activation_function_layer(LINEAR,2);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE,2);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE_SYMMETRIC,2);
		//rede_neural->set_activation_function_layer(SIGMOID,2);
		rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC,2);
		//rede_neural->set_activation_function_layer(SIGMOID_STEPWISE,2);
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC_STEPWISE,2);
		//rede_neural->set_activation_function_layer(ELLIOT,2);
		//rede_neural->set_activation_function_layer(ELLIOT_SYMMETRIC,2);
		//rede_neural->set_activation_function_layer(GAUSSIAN,2);
		//rede_neural->set_activation_function_layer(GAUSSIAN_SYMMETRIC,2);
		//rede_neural->set_activation_function_layer(COS_SYMMETRIC,2);
		//rede_neural->set_activation_function_layer(SIN_SYMMETRIC,2);

		//------ALGORITMO DE ATIVACAO LAYER3
		//rede_neural->set_activation_function_layer(LINEAR,3);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE,3);
		//rede_neural->set_activation_function_layer(LINEAR_PIECE_SYMMETRIC,3);
		//rede_neural->set_activation_function_layer(SIGMOID,3);
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC,3);
		//rede_neural->set_activation_function_layer(SIGMOID_STEPWISE,3);
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC_STEPWISE,3);
		//rede_neural->set_activation_function_layer(ELLIOT,3);
		//rede_neural->set_activation_function_layer(ELLIOT_SYMMETRIC,3);
		//rede_neural->set_activation_function_layer(GAUSSIAN,3);
		//rede_neural->set_activation_function_layer(GAUSSIAN_SYMMETRIC,3);
		//rede_neural->set_activation_function_layer(COS_SYMMETRIC,3);
		//rede_neural->set_activation_function_layer(SIN_SYMMETRIC,3);

		//FUNCAO DE PARADA
		//rede_neural->set_train_stop_function(STOPFUNC_BIT);


		rede_neural->set_learning_rate(aprendizagem);

		rede_neural->set_activation_steepness_layer(0.35,1);
		rede_neural->set_activation_steepness_layer(0.35,2);
		//rede_neural->set_activation_steepness_layer(0.33,3); //

		//rede_neural->set_activation_steepness(0.8,2,1);
		//rede_neural->set_activation_steepness(0.1,2,3);

		//rede_neural->set_quickprop_decay(-0.0002);
		//rede_neural->set_quickprop_mu(1.75);
		//rede_neural->set_bit_fail_limit(0.3);
		//rede_neural->set_bit_fail_limit(rede_neural->get_bit_fail_limit() / cortex->caracteristicas[grupo]->complexibilidade);
		rede_neural->init_weights(*dados);
		//rede_neural->set_scaling_params(*dados,-1,1,-1,1);
		//rede_neural->save(ann_file);
	}else{
		//rede_neural->print_connections();
		//printf("%p\n", dados);
		cortex->caracteristicas[grupo]->status = CARACTERISTICAS_ANALISADAS;
		//printf("MSE: %f\n", rede_neural->get_MSE() );
	}
	return rede_neural;
}

training_data *CLobo::getDados(){

	if(dados)
		return dados;

	dados = new training_data();
	dados->read_train_from_file(train_file);

	return dados;
}

int CLobo::getNumDados(){
return dados->length_train_data();
}

void CLobo::analisaDados(){
	//Para treinar o bloco é preciso linhas...
	if( !getCaracteristicas() || !getRespostas() || !getRedeNeural() || !getDados())
		return;

	rede_neural->set_callback(callback_html,this);

	if(!memcmp(cortex->caracteristicas[grupo]->maximos,zero,num_caracteristicas*sizeof(fann_type))){
		printf("Sem dados para treinar.\n");
		return;
	}

	printf("Treinando..., margem de erro %.8f...\n", margem_de_erro);
	training_data *dados_aleatorios;
	dados_aleatorios = new training_data();
	dados_aleatorios->set_train_data(dados->length_train_data(),
						  num_caracteristicas, dados->get_input(),
						  num_respostas, dados->get_output());

	dados_aleatorios->shuffle_train_data();
	//dados_aleatorios->scale_train_data(-1,1);
	rede_neural->train_on_data(*dados_aleatorios, turnos, turnos_por_amostra, margem_de_erro);
	delete dados_aleatorios;

	rede_neural->save("/tmp/net");
	rede_neural->save(ann_file);

	dados->save_train("/tmp/train");
	dados->save_train(train_file);

	//rede_neural->print_connections();
	printf("%0.10f\n",rede_neural->test_data(*dados));
	cortex->caracteristicas[grupo]->status = CARACTERISTICAS_ANALISADAS;

	return;
}

void CLobo::generalizaDados(){
	//Para treinar o bloco é preciso linhas...
	if( !getCaracteristicas() || !getRespostas() || !getRedeNeural() || !getDados())
		return;

	fann_type MSE;

	//MSE = rede_neural->test_data(*dados);
	//rede_neural->reset_MSE();
	//MSE = rede_neural->test_data(*dados);
	MSE = rede_neural->get_MSE();
	//printf("%f\n",MSE);

	if(cortex->caracteristicas[grupo]->status > CARACTERISTICAS_ANALISADAS)
		return;
	else if(cortex->caracteristicas[grupo]->status < CARACTERISTICAS_ANALISADAS)
		analisaDados();

	printf("MSE: %f, generalizando...\n", MSE);
	fann_type **caracteristicas = getCaracteristicas();
	fann_type **respostas = getRespostas();
	fann_type *resposta;
	int	num_dados = dados->length_train_data();
	int tamanho_respostas = num_respostas * sizeof(fann_type);
	//fann_type dado[num_dados];

	while(num_dados--){
		//memcpy(dado, caracteristicas[num_dados], num_dados*sizeof(fann_type));
		//rede_neural->scale_input(dado);
		resposta = rede_neural->run(caracteristicas[num_dados]);
		//rede_neural->descale_output(resposta);
		memcpy(respostas[num_dados], resposta, tamanho_respostas);
	}
	cortex->caracteristicas[grupo]->status = CARACTERISTICAS_GENERALIZADAS;
	return;
}
/*

void CLobo::deduzDados(){
	if( !getCaracteristicas() || !getRespostas() || !getRedeNeural() || !getDados())
		return;

	//fann_type MSE;

	//MSE = rede_neural->test_data(*dados);

	//if(MSE > 0.009)
	 generalizaDados();

	//printf("MSE: %f, generalizando...\n", MSE);

	fann_type **caracteristicas = getCaracteristicas();
	fann_type **respostas = getRespostas();
	fann_type *resposta;
	CCaracteristicas *entradas, *tmp;
	int	num_dados = dados->length_train_data();
	entradas = NULL;
	int num_entradas = 0;
	while(num_dados--){
		if(entradas){
			for(tmp = entradas ; tmp ; tmp = tmp->next)
				if(tmp->Compara(caracteristicas[num_dados])){
					print_entradas(tmp->entradas, num_caracteristicas);
					print_entradas(caracteristicas[num_dados], num_caracteristicas);
					break;
				}

			if(!tmp){
				tmp = new CCaracteristicas(caracteristicas[num_dados], num_caracteristicas);
				tmp->next = entradas;
				entradas = tmp;
				num_entradas++;
			}

		}
		entradas = new CCaracteristicas(caracteristicas[num_dados], num_caracteristicas);
	}
	printf("%d\n", num_entradas);

	return;
}
*/

fann_type **CLobo::getCaracteristicas(){
	return dados->get_input();
}

fann_type **CLobo::getRespostas(){
	return dados->get_output();
}

CLobo *CLobo::setCaracteristicas(int _num_caracteristicas, fann_type **caracteristicas){
	num_caracteristicas = _num_caracteristicas;
	dados->set_train_data(getNumDados(), num_caracteristicas, caracteristicas, num_respostas, getRespostas() );
	return this;
}

CLobo *CLobo::setRespostas(int _num_respostas, fann_type **respostas){
	num_respostas = _num_respostas;
	dados->set_train_data(getNumDados(), num_caracteristicas, getCaracteristicas(), num_respostas, respostas );
	return this;
}

//
//void CLobo::identificaLinhas(){
//
// 	if(!getDadosIdent() || !getDados() || !getRedeNeural() || !getPaginas())
// 		return;
//	/*
// 	 * Pega areas identificadas lembradas
// 	 * Analiza o layout
// 	 * Cria lista de areas lembradas vinculando-as ao dado
// 	 * Marca dados com a mesma area que o dado identificado e remove-o da lista
// 	 *
// 	 * */
// 	 analisaLayout();
//
//    if(!getAreasIdent()){
//    	printf("Linhas identificadas não reconhecidas.\n");
//    	return;
//    }
//
// 	paginas->getInfo();
//	int width = paginas->info.width;
//	int height = paginas->info.height;
//
// 	fann_type **dados_in  = dados->get_input();
// 	fann_type **dados_out = dados->get_output();
//
// 	int num_dados = dados->length_train_data();
//
// 	int count_dados, count_ident, dados_ident;
// 	fann_type *in, *out;
// 	area_t *areas_dados, *area_ident, *area_dado, area;
//
// 	count_dados = num_dados;
// 	count_ident = num_areas_ident;
// 	dados_ident = 0;
// 	printf("%d %d\n", count_dados, count_ident);
// 	areas_dados = (area_t*)malloc(sizeof(area_t)*num_dados+1);
// 	while(count_dados--){
// 		in  = dados_in[count_dados];
// 		out = rede_neural->run(in);
//
// 		area.w = (int)round(out[0] * width);
//		area.h = (int)round(out[1] * height);
//		area.t = (int)round(out[2] * height);
//		area.l = (int)round(out[3] * width);
//		area.tipo = 0;
//
//		memcpy(&areas_dados[count_dados],&area,sizeof(area_t));
//		areas_dados[count_dados].dados = dados_out[count_dados];
//		areas_dados[count_dados].next = &areas_dados[count_dados+1];
//		areas_dados[count_dados+1].prev = &areas_dados[count_dados];
//	}
//	areas_dados[count_dados-1].next = NULL;
//	for(area_ident = areas_ident; area_ident; area_ident = area_ident->next){
//		for(area_dado = areas_dados; area_dado;area_dado = area_dado->next){
//			//if(!memcmp(area_dado, area_ident, sizeof(int)*4)){
//			if(area_ident->w == area_dado->w &&
//			   area_ident->h == area_dado->h &&
//			   area_ident->t == area_dado->t &&
//			   area_ident->l == area_dado->l){
//				dados_ident++;
//				//printf("%f\n", area_ident->tipo);
//				area_dado->dados[4] = area_ident->tipo;
//				if(area_dado->prev){
//					area_dado->prev->next = area_dado->next;
//					area_dado->next->prev = area_dado->prev;
//				}
//			}
//		}
//	}
//	printf("%d linhas identificadas de %d total.\n",dados_ident, num_dados);
//	printf("Salvando dados em %s\n", train_file);
//	dados->save_train(train_file);
//	analisaLayout();
//	geraHTML(NULL);
//}
//
//void CLobo::geraHTML(char *cabecalho){
//	if(!getPaginas() || !getRedeNeural() || !getDados() || !getAreas())
//		return;
//
//	area_t *area;
//
//	paginas->getInfo();
//
//	const char *tipos[] = { "#FFF", "#F55","#5F5","#55F","#FF0","#0FF","#AAA" };
//
//	FILE *html = fopen("/tmp/mapa.html","w");
//	int width = paginas->info.width;
//	int height = paginas->info.height;
//
//	fprintf(html,"<html>\n<style>div { border: 1px solid black; }</style>\n<body>\n<div style='width:%d;height:%d;top:50;left:370;position:absolute;'>\n",
//	width / 3, height / 3);
//
//	area = areas;
//	while(area){
//		fprintf(html, "<div style='width:%d;height:%d;top:%d;left:%d;position:absolute;background:%s;'>&nbsp;</div>\n",
//								area->w / 3,
//								area->h / 3,
//								area->t / 3,
//								area->l / 3,
//								tipos[(int)round(area->tipo*7)]);
//		area = area->next;
//	}
//	//char conexoes[20480];
//	//rede_neural->print_connections(conexoes);
//	fprintf(html,"</div><pre>%s%d areas total, %d areas unicas</pre>\n", cabecalho?cabecalho:"", dados->length_train_data(), num_areas);
//	fclose(html);
//	return;
//}
//
//area_t *CLobo::getAreas(){
//	if(areas)
//		return areas;
//
//	if(!getPaginas() || !getDados() || !getRedeNeural())
//		return NULL;
//
//	info_t *info_pagina = paginas->getInfo();
//	int width = info_pagina->width;
//	int height = info_pagina->height;
//	int count;
//	fann_type **in = dados->get_input();
//	fann_type *i, *o;
//	area_t *area, *ultima_area, area_dado;
//
//	num_areas = 0;
//	count = dados->length_train_data();
//	//printf("%d %d\n", num_areas, count);
//	while(count--){
//		i = in[count];
//		o = rede_neural->run(i);
//
//		area_dado.w = (int)round(o[0] * width);
//		area_dado.h = (int)round(o[1] * height);
//		area_dado.t = (int)round(o[2] * height);
//		area_dado.l = (int)round(o[3] * width);
//
//		//free(o);
//		if(areas){
//			area = areas;
//
//			while(area){
//				if(area->w == area_dado.w &&
//				   area->h == area_dado.h &&
//				   area->t == area_dado.t &&
//				   area->l == area_dado.l){
//					area->c++;
//					//area_dado.tipo = (o[4] + area_dado.tipo) / 2;
//					break;
//				}
//				area = area->next;
//			}
//			if(!area){
//				area_dado.next = areas;
//				areas = (area_t*)malloc(sizeof(area_t));
//				memcpy(areas,&area_dado,sizeof(area_t));
//				num_areas++;
//			}
//		}else{
//			areas = (area_t*)malloc(sizeof(area_t));
//			memcpy(areas,&area_dado,sizeof(area_t));
//			areas->next = NULL;
//			areas->c = 1;
//			num_areas++;
//		}
//	}
//
//	area = areas;
//	areas = (area_t*)malloc(sizeof(area_t)*num_areas);
//	count=0;
//	while(area){
//		memcpy(&areas[count],area,sizeof(area_t));
//		areas[count].next = &areas[count+1];
//
//		ultima_area = area;
//		area = area->next;
//		free(ultima_area);
//		count++;
//	}
//	areas[count-1].next = NULL;
//	return areas;
//}
//
//area_t *CLobo::getAreasIdent(){
//	if(areas_ident)
//		return areas_ident;
//
//	if(!getPaginas() || !getDadosIdent() || !getRedeNeural() )
//		return NULL;
//
//	info_t *info_pagina = paginas->getInfo();
//	int width = info_pagina->width;
//	int height = info_pagina->height;
//	int count = dados_ident->length_train_data();
//	fann_type **in = dados_ident->get_input();
//	fann_type **out = dados_ident->get_output();
//	fann_type *i, *o;
//	area_t *area, *ultima_area, area_dado;
//	//printf("%d\n", count);
//	num_areas = 0;
//	while(count--){
//		i = in[count];
//		o = rede_neural->run(i);
//
//		area_dado.w = (int)round(o[0] * width);
//		area_dado.h = (int)round(o[1] * height);
//		area_dado.t = (int)round(o[2] * height);
//		area_dado.l = (int)round(o[3] * width);
//
//		//
//
//		//free(o);
//		if(areas_ident){
//			area = areas_ident;
//			while(area){
//				if(area->w == area_dado.w &&
//				   area->h == area_dado.h &&
//				   area->t == area_dado.t &&
//				   area->l == area_dado.l){
//					area->c++;
//					area_dado.tipo = (out[count][4] + area_dado.tipo) / 2;
//					printf("%f\n", area_dado.tipo);
//					break;
//				}
//				area = area->next;
//			}
//			if(!area){
//				area_dado.next = areas_ident;
//				area_dado.tipo = out[count][4];
//				areas_ident = (area_t*)malloc(sizeof(area_t));
//				memcpy(areas_ident,&area_dado,sizeof(area_t));
//				num_areas_ident++;
//			}
//		}else{
//			areas_ident = (area_t*)malloc(sizeof(area_t));
//			memcpy(areas_ident,&area_dado,sizeof(area_t));
//			areas_ident->tipo = out[count][4];
//			areas_ident->next = NULL;
//			areas_ident->c = 1;
//			num_areas_ident++;
//		}
//	}
//
//	area = areas_ident;
//	areas_ident = (area_t*)malloc(sizeof(area_t)*num_areas_ident);
//	count=0;
//	while(area){
//		memcpy(&areas_ident[count],area,sizeof(area_t));
//		areas_ident[count].next = &areas_ident[count+1];
//
//		ultima_area = area;
//		area = area->next;
//		free(ultima_area);
//		count++;
//	}
//	areas_ident[count-1].next = NULL;
//	return areas_ident;
//}


