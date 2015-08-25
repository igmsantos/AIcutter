/*
 * Cerebro.cpp
 *
 *  Created on: 14/06/2010
 *      Author: ivan
 */

#include "include/Cerebro.h"


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
{
	/* Command is the control command to the terminal */
	sprintf(buffer, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
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
	 void *_cerebro){

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
	 static float aprendizagem_inicial = 0;

	 static bool set_steepness = false;
	 static bool set_aprendizagem = false;

	 static training_data *dados_teste = NULL;

	 char buffer[20000];
	 char MSE_buffer[64];
	 char bit_fail_buffer[64];
	 float diferenca_MSE;
	 float MSE;
	 int bit_fail = 0;
	 int diferenca_bit_fail = 0;

	 CCerebro *cerebro = (CCerebro*)_cerebro;

	 if(dados_teste){
		//rede_neural.reset_MSE();
		//printf("%f ", rede_neural.get_MSE());
		MSE = rede_neural.test_data(*dados_teste);
		//printf("%f\n", MSE);
	 }
	MSE = rede_neural.get_MSE();

	 bit_fail = rede_neural.get_bit_fail();

	//rede_neural.reset_MSE();

	//printf("%f %f %d\n", MSE, last_MSE, (MSE - last_MSE) > 0.000001);
	if(last_MSE)
		diferenca_MSE = (last_MSE - MSE);
	else
		diferenca_MSE = 0;

	if(last_bit_fail)
		diferenca_bit_fail = last_bit_fail - bit_fail;
	else
		diferenca_bit_fail = 0;

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

	//printf("%d\n", bit_fail_counter);
	//printf("%f %f\n",last_MSE, diferenca_MSE);
	if(last_MSE){
		fprintf(plot, "%d %f %f %d %d\n",turnos, MSE, diferenca_MSE, bit_fail_counter, MSE_counter);
		fflush(plot);
		total = total + diferenca_MSE;
	}

	last_MSE = MSE;
	last_bit_fail = bit_fail;

	if(set_aprendizagem){
		if(!aprendizagem_inicial)
			aprendizagem_inicial = cerebro->rede_neural->get_learning_rate();

		aprendizagem = (1-((float)(MSE_counter + bit_fail_counter) / 200)) * aprendizagem_inicial;
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

		if(dados_teste)
			free(dados_teste);

		if(plot)
			fclose(plot);
/*
		dados_teste = new training_data();
		if(cerebro->getDadosIdentificadosEscalados()){
			dados_teste->set_train_data(cerebro->num_dados_identificados,
					cerebro->num_caracteristicas, cerebro->dados_identificados_escalados,
					cerebro->num_caracteristicas, cerebro->dados_identificados_escalados);
			printf("Utilizando massa de testes identificada com %d casos.\n",cerebro->num_dados_identificados );
		}else if(cerebro->getDadosEscalados()){
			dados_teste->set_train_data(cerebro->num_dados,
						cerebro->num_caracteristicas, cerebro->dados_escalados,
						cerebro->num_caracteristicas, cerebro->dados_escalados);
			printf("Utilizando massa de testes não identificada com %d casos.\n",cerebro->num_dados );
		}else{
			printf("Sem dados para treinar.\n");
			delete dados_teste;

			return -1;
		}
*/
		printf("Plotando em %s.\n",cerebro->plot_file);
		plot = fopen(cerebro->plot_file, "w");
	}
	loops = 0;

	memset(buffer,0,20000);

	//clear_screen();

	cor_nivel(bit_fail_counter,bit_fail_buffer);
    cor_nivel(MSE_counter,MSE_buffer);

	//printf("%f\n",round((float)bit_fail_counter/10));
	sprintf((char*)(buffer + strlen(buffer)), "%08d turnos, precisão %1.8f(%-1.8f)(%s%%), nivel de adaptacao %d(%d)(%s%%)",
											  turnos, MSE, diferenca_MSE, MSE_buffer, bit_fail, diferenca_bit_fail, bit_fail_buffer);
	sprintf((char*)(buffer + strlen(buffer)),", depuracao %-1.6f, aprendizagem %f, passo %f", total,last_aprendizagem,steepness);
	sprintf((char*)(buffer + strlen(buffer)),"\n");
	printf(buffer);
	//textcolor(RESET, WHITE, BLACK);
	//rede_neural.print_connections(buffer);
	//printf(buffer);


	if(bit_fail_counter == BIT_FAIL_COUNTER && MSE_counter == MSE_COUNTER){
			printf("Treino completado em %08d, adaptacao %d, MSE %f\n",turnos, bit_fail, MSE);
			return -1;
		}

	return 0;
}

CCerebro::CCerebro(){

	memset(zero,0,ZERO_SIZE);

	num_dados = 0;
	num_caracteristicas = 0;

	dados_identificados =
	dados_identificados_escalados =
	dados_originais =
	dados_escalados =
	dados_analisados =
	dados_generalizados =
	dados_deduzidos =
	dados_externos =
	dados_aleatorios = NULL;

	identificacao = NULL;
	maximos = NULL;
	minimos = NULL;
	diferencas = NULL;
	totais = NULL;
	limites = NULL;
	medias = NULL;
	meios = NULL;
	cascade = false;

	turnos = 5000;
 	turnos_por_amostra = 1;
 	margem_de_erro 	= 0.00000001;//0.00000001; //0.000000000121076
 	aprendizagem 	= 0.3;
 	conectividade 	= 1;

 	if(cascade){
 		num_neuronios_invisiveis = 5;
 		num_camadas = 2;
 	}else{
 		num_neuronios_invisiveis = 20;
 		num_camadas = 3;
 	}
 	//Parametros da rede neural
 	num_caracteristicas = 0;
 	camadas = NULL;
 	rede_neural = NULL;

}

neural_net *CCerebro::getRedeNeural(){
	if(rede_neural)
		return rede_neural;

	if(!getDadosOriginais() && !getDadosIdentificados()){
		printf("Sem rede neural.\n");
		return NULL;
	}

	rede_neural = new neural_net();
	if(!rede_neural->create_from_file(ann_file)){
	//if(1){
		activation_function_enum funcoes[] = {  SIGMOID_SYMMETRIC , ELLIOT_SYMMETRIC, SIN_SYMMETRIC, GAUSSIAN };
	 	camadas = (unsigned int *)malloc(sizeof(unsigned int)*num_camadas);
	 	if(cascade){
	 		camadas[0] = num_caracteristicas;
	 		camadas[1] = num_caracteristicas;
	 		rede_neural->create_shortcut_array(num_camadas,camadas);
	 		rede_neural->set_cascade_max_out_epochs(10000);
	 		rede_neural->set_activation_function_output(LINEAR);
	 		rede_neural->set_activation_steepness_output(0.2);
	 		rede_neural->set_cascade_activation_functions(funcoes, 4);

	 	}else{
	 		camadas[0] = num_caracteristicas - offset;
			camadas[1] = num_neuronios_invisiveis;
			camadas[2] = offset;

			rede_neural->create_sparse_array(conectividade,num_camadas,camadas);
			rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC,1);
			rede_neural->set_activation_function_layer(LINEAR,2);

			rede_neural->set_activation_steepness_layer(0.3,1); //
			//rede_neural->set_activation_steepness_layer(0.3,1);
			/*rede_neural->set_activation_steepness_layer(0.25,3); //
			rede_neural->set_activation_steepness_layer(0.25,4);*/
	 	}

		//rede_neural->create_sparse_array(conectividade,num_camadas,camadas);
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
		//rede_neural->set_activation_function_layer(GAUSSIAN,1);
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
		//rede_neural->set_activation_function_layer(SIGMOID_SYMMETRIC,2);
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


		//rede_neural->set_learning_rate(aprendizagem);

		//rede_neural->set_train_error_function(ERRORFUNC_LINEAR);
		//rede_neural->set_activation_steepness(0.8,2,1);
		//rede_neural->set_activation_steepness(0.1,2,3);

		//rede_neural->set_quickprop_decay(-0.0002);
		//rede_neural->set_quickprop_mu(1.75);
		//rede_neural->set_bit_fail_limit(0.3);
		//rede_neural->set_bit_fail_limit(rede_neural->get_bit_fail_limit() / cortex->caracteristicas[grupo]->complexibilidade);
		//rede_neural->set_scaling_params(*dados,-1,1,-1,1);

		//rede_neural->save(ann_file);
		turnos *= 3;

		printf("Rede neural criada: %d caracteristicas, %d neuronios invisiveis, %d%% conectados.\n",
 			num_caracteristicas, num_neuronios_invisiveis, (int)round(100 * conectividade));

	}else{
		/*if(getDadosEscalados()){
			dados_analisados = dados_escalados;
		}else if(getDadosIdentificados()){
			dados_analisados = dados_identificados_escalados;
		}else{
			printf("Sem rede neural\n.");
			return NULL;
		}*/

		rede_neural->print_connections();
		//printf("%p\n", dados);
		printf("MSE: %f\n", rede_neural->get_MSE() );
	}
	rede_neural->print_parameters();
	return rede_neural;
}
CDocumentoIA *CCerebro::getDocumento(){
	return documento;
}
CCerebro *CCerebro::setAtencao(CDocumentoIA*objeto,int tipo_objeto){
	documento = (CDocumentoIA*)objeto;
 	char *arquivo = documento->getArquivo();


 	sprintf(ann_file,"%s", arquivo );
 	sprintf(train_file,"%s",  arquivo );
 	sprintf(plot_file,"%s",  arquivo );
 	sprintf(map_file,"%s",  arquivo );

 	sprintf(&ann_file[strlen(arquivo)-4] 	, ".net");
 	sprintf(&train_file[strlen(arquivo)-4]	, ".train");
 	sprintf(&plot_file[strlen(arquivo)-4]	, ".plot");
 	sprintf(&map_file[strlen(arquivo)-4]	, ".map");

	return this;
}
fann_type *CCerebro::getMaximos(){
	if(maximos)
		return maximos;

	fann_type **dados;
	int i;
/*
	if(getDadosIdentificados()){
		dados = dados_identificados;
		i = num_dados_identificados;

	}else*/ if(getDadosOriginais()){
		dados = dados_originais;
		i = num_dados;

	}else{
		printf("Sem maximos.\n");
		return NULL;
	}

	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	maximos = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);
	memset(maximos,0,num_caracteristicas * sizeof(fann_type));

	while(u--)
		maximos[u] = 0;

	count=i;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados[count];
		//print_entradas(caracteristicas,num_caracteristicas);
		//printf("%d %p\n", count, caracteristicas);
		while(u--){
			if(caracteristicas[u] > maximos[u])
				maximos[u] = caracteristicas[u];		}
	}
	printf("Máximos:\t");
	print_entradas(maximos,num_caracteristicas);

	return maximos;
}
fann_type *CCerebro::getMinimos(){
	if(minimos)
		return minimos;

	fann_type **dados;
	int i;

	if(!getMaximos())
			return NULL;

	/*else if(getDadosIdentificados()){
		dados = dados_identificados;
		i = num_dados_identificados;
	}else*/ if(getDadosOriginais()){
		dados = dados_originais;
		i = num_dados;
	}else{
		printf("Sem minimos.\n");
		return NULL;
	}

		int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;

	minimos = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	memcpy(minimos, maximos, sizeof(fann_type)*num_caracteristicas);

	count=i;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados[count];
		while(u--){
			if(caracteristicas[u] < minimos[u])
				minimos[u] = caracteristicas[u];
		}
	}

	printf("Mínimos:\t");
	print_entradas(minimos,num_caracteristicas);

	return minimos;
}
fann_type *CCerebro::getTotais(){
	if(totais)
		return totais;

	fann_type **dados;
	int i;
	/*if(getDadosIdentificados()){
		dados = dados_identificados;
		i = num_dados_identificados;
	}else*/ if(getDadosOriginais()){
		dados = dados_originais;
		i = num_dados;
	}else{
		printf("Sem totais.\n");
		return NULL;
	}

	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	totais = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	memset(totais,0,sizeof(fann_type)*num_caracteristicas);

	count=i;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados[count];
		while(u--)
			totais[u] += caracteristicas[u];
	}

	printf("Totais:\t\t");
	print_entradas(totais,num_caracteristicas);

	return totais;
}
fann_type *CCerebro::getMedias(){
	if(medias)
		return medias;

	if(!getTotais()){
		printf("Sem totais.\n");
		return NULL;
	}
	int i,u;

	/*if(getDadosIdentificados()){
			i = num_dados_identificados;
		}else*/ if(getDadosOriginais()){
			i = num_dados;
		}else{
			printf("Sem totais.\n");
			return NULL;
		}

	u=num_caracteristicas;
	medias = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	while(u--)
		medias[u] = totais[u] / i;

	printf("Medias:\t\t");
	print_entradas(medias,num_caracteristicas);

	return medias;
}
fann_type *CCerebro::getDesvios(){
	if(desvios)
		return desvios;

	if(!getMedias()){
		printf("Sem desvios.\n");
		return NULL;
	}

	fann_type **dados;
	int i;
	/*if(getDadosIdentificados()){
		dados = dados_identificados;
		i = num_dados_identificados;
	}else*/ if(getDadosOriginais()){
		dados = dados_originais;
		i = num_dados;
	}else{
		printf("Sem desvios.\n");
		return NULL;
	}
	int u, count;
	fann_type *caracteristicas;

	u=num_caracteristicas;
	desvios = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	while(u--)
		desvios[u] = 0;

	count=i;
	while(count--){
		u=num_caracteristicas;
		caracteristicas = dados[count];
		while(u--){
			desvios[u] += pow(caracteristicas[u] - medias[u],2);
		}
		//print_entradas(desvios,num_caracteristicas);
	}

	u=num_caracteristicas;
	while(u--)
		desvios[u] = sqrt( desvios[u] / i );

	printf("Desvios padrão:\t");
	print_entradas(desvios,num_caracteristicas);
	return desvios;
}
int 	  *CCerebro::getLimites(){
	if(limites)
		return limites;

	if( !getMeios() || !getDesvios() || !getMaximos() ){
		return NULL;
	}
	int u;

	limites = (int*)malloc(sizeof(int)*num_caracteristicas);
	memset(limites,0,sizeof(int)*num_caracteristicas);
	printf("Limites:\t");
	u = 0;
	while(u < num_caracteristicas){
		if(desvios[u] == 0)
			limites[u] = LIMITE_NULO;
//		else if(maximos[u] <= 1){
//		///	if(medias[u] != meios[u])
//			//	limites[u] = LIMITE_ORIGINAL;//totais[u] / num_dados;
//			//else
//				limites[u] = LIMITE_RESCALADO;
//
//		}
		else if(maximos[u])
			//limites[u] = LIMITE_NORMALIZADO;
			limites[u] = LIMITE_DESVIO;
			//limites[u] = LIMITE_ORIGINAL;

		printf(" %d", limites[u]);
		u++;
	}
	printf("\n");
	//print_entradas(limites,num_caracteristicas);

	return limites;

}
fann_type *CCerebro::getMeios(){
	if(meios)
		return meios;

	if(!getMaximos() || !getMinimos())
		return NULL;

	int u;

	u=num_caracteristicas;
	meios = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	while(u--)
		meios[u] = (maximos[u] + minimos[u])/2;

	printf("Meios:\t\t");
	print_entradas(meios,num_caracteristicas);

	return meios;

}
fann_type *CCerebro::getDiferencas(){
	if(diferencas)
		return diferencas;

	if(!getMaximos() || !getMinimos())
		return NULL;

	int u;

	u=num_caracteristicas;
	diferencas = (fann_type*)malloc(sizeof(fann_type)*num_caracteristicas);

	while(u--)
		diferencas[u] = maximos[u] - minimos[u];

	printf("Diferenças:\t");
	print_entradas(diferencas,num_caracteristicas);

	return diferencas;

}

fann_type **CCerebro::getDadosOriginais(){

	if(dados_originais)
		return dados_originais;

	int count, i, u;

	fann_type **dados[4], *caracteristicas,  *caracteristicas_originais, max;
	int num_entradas[4], grupo;

	dados[0] = documento->getCaracteristicasVisuais();
	dados[1] = documento->getCaracteristicasEspaciais();
	dados[2] = documento->getCaracteristicasEstatisticas();
	dados[3] = documento->getCaracteristicasClassificatorias();

	num_caracteristicas = 0;
	num_caracteristicas += num_entradas[0] = documento->num_caracteristicas_visuais;
	num_caracteristicas += num_entradas[1] = documento->num_caracteristicas_espaciais;
	num_caracteristicas += num_entradas[2] = documento->num_caracteristicas_estatisticas;
	num_caracteristicas += num_entradas[3] = offset = documento->num_caracteristicas_classificatorias;

	num_dados = documento->num_linhas;

	identificacao   = (int*)malloc(num_dados * sizeof(int));
	dados_originais = (fann_type**)malloc(num_dados * sizeof(fann_type*));
	caracteristicas_originais = (fann_type*)malloc( num_caracteristicas * num_dados * sizeof(fann_type) );

	count = num_dados;
	num_dados = 0;
	// ENQUANTO HOUVER LINHAS
	while(count--){
		//SALVA PONTEIRO NA LISTA DE DADOS
		dados_originais[num_dados] = caracteristicas_originais;
		grupo = 4;
		u = max = 0;
		while(grupo--){
			caracteristicas = dados[grupo][count];
			if(grupo == 3){

				i = num_entradas[grupo];
				while(i--)
					if(caracteristicas[i] > 0 )
						if(max < caracteristicas[i] ){
							max = caracteristicas[i];
							u = i;
						}
			}
			memcpy(caracteristicas_originais ,caracteristicas , sizeof(fann_type) * num_entradas[grupo] );
			caracteristicas_originais += num_entradas[grupo];
		}
		identificacao[num_dados++] = u;

		//print_entradas(caracteristicas_originais-num_caracteristicas,num_caracteristicas);

	}

	printf("Dados: %d dados com %d caracteristicas (%d visuais, %d espaciais, %d estatisticas e %d classificatorias).\n",
			num_dados,
			num_caracteristicas,
			documento->num_caracteristicas_visuais,
			documento->num_caracteristicas_espaciais,
			documento->num_caracteristicas_estatisticas,
			documento->num_caracteristicas_classificatorias);

	return dados_originais;
}

fann_type **CCerebro::getDadosIdentificados(){

	if(dados_identificados)
		return dados_identificados;

	if(!getDadosOriginais())
		return NULL;

	int count, i;

	i = documento->num_linhas;
	count = 0;
	while(i--)
		if(identificacao[i])
			count++;

	num_dados_identificados = count;

	dados_identificados = (fann_type**)malloc(num_dados_identificados * sizeof(fann_type*));

	i = num_dados;
	count = num_dados_identificados;
	while(i--)
		if(identificacao[i]){
			//print_entradas(dados_originais[i], num_caracteristicas);
			//printf("%d %d\n", i, identificacao[i]);
			dados_identificados[--count] = dados_originais[i];
		}

	printf("Dados identificados: %d dados identificados.\n",
			num_dados_identificados);

	return dados_identificados;
}

fann_type **CCerebro::getDadosEscalados(){
	if(dados_escalados)
		return dados_escalados;

	if(!getDadosOriginais() || !getLimites() || !getMaximos()){
		printf("Sem dados escalados.\n");
		return NULL;
	}
	int u,count;

	u = num_caracteristicas;
	while(u--){
		switch(limites[u]){
		case LIMITE_ORIGINAL:///Sem calculos (linear)
			break;
		case LIMITE_RESCALADO:
			if(!getMaximos())
				return NULL;
			break;
		case LIMITE_NORMALIZADO:///Valor normalizado entre -1 e 1
			if(!getMeios() || !getDiferencas())
				return NULL;
			break;
		case LIMITE_DESVIO:
			if(!getMedias() || !getDesvios())
				return NULL;
			break;
		}
	}

	fann_type *caracteristicas, *caracteristicas_originais;
	dados_escalados = (fann_type**)malloc(sizeof(fann_type*)*num_dados);
	caracteristicas = (fann_type*)malloc(sizeof(fann_type)*num_dados*num_caracteristicas);

	count = num_dados;
	while(count--){
		dados_escalados[count] = caracteristicas;
		caracteristicas_originais = dados_originais[count];
		u=num_caracteristicas;
		while(u--){
			switch(limites[u]){
				case LIMITE_ORIGINAL:///Sem calculos (linear)
					caracteristicas[u] = caracteristicas_originais[u];
					break;
				case LIMITE_RESCALADO:///Valor calculado a partir da razao com o maximo, entre -1 e 1
					caracteristicas[u] = ((caracteristicas_originais[u] / maximos[u]) * 2) - 1;
					break;
				case LIMITE_NORMALIZADO:///Valor normalizado entre -1 e 1
					//caracteristicas[u] = (caracteristicas_originais[u] - minimos[u]) / diferencas[u];
					caracteristicas[u] = (caracteristicas_originais[u] - meios[u]) / (diferencas[u] / 2);
					break;
				case LIMITE_DESVIO:
					caracteristicas[u] = (caracteristicas_originais[u] - medias[u]) / desvios[u];
					break;
				case LIMITE_NULO:
					caracteristicas[u] = 0;
				break;
			}
		}
		caracteristicas += num_caracteristicas;
		//print_entradas(caracteristicas-num_caracteristicas,num_caracteristicas);
	}

	return dados_escalados;
}

fann_type **CCerebro::getDadosIdentificadosEscalados(){
	if(dados_identificados_escalados)
		return dados_identificados_escalados;

	if(!getDadosIdentificados() || !getDadosEscalados() || !getLimites() || !getMaximos()){
		printf("Sem dados identificados escalados.\n");
		return NULL;
	}

	int count, i;

	dados_identificados_escalados = (fann_type**)malloc(num_dados_identificados * sizeof(fann_type*));

	i = num_dados;
	count = num_dados_identificados;
	while(i--)
		if(identificacao[i])
			dados_identificados_escalados[--count] = dados_escalados[i];

	return dados_identificados_escalados;
}

fann_type **CCerebro::getDadosAnalisados(){
	if(!getRedeNeural() || !getMaximos())
		return NULL;

	if(dados_analisados)
		return dados_analisados;

	if(!cascade)
		rede_neural->set_callback(callback_html,this);

	if(!memcmp(maximos,zero,num_caracteristicas*sizeof(fann_type))){
		printf("Sem dados para treinar.\n");
		return NULL;
	}


	printf("Treinando..., margem de erro %.8f...\n", margem_de_erro);
	training_data *dados_aleatorios;
	dados_aleatorios = new training_data();

	fann_type **dados,*caracteristicas;
	int i, u, count;

	if(getDadosIdentificadosEscalados()){
		i = num_dados_identificados;
		dados = dados_identificados_escalados;
	}else if(getDadosEscalados()){
		i = num_dados;
		dados = dados_escalados;
	}else{
		printf("Sem dados para treinar.\n");
		delete dados_aleatorios;
		return NULL;
	}

	fann_type **in, **out;
	in = (fann_type**)malloc(sizeof(fann_type*)*i);
	out = (fann_type**)malloc(sizeof(fann_type*)*i);
	//caracteristicas = (fann_type*)malloc(sizeof(fann_type)*i*num_caracteristicas);
	//memcpy(caracteristicas,dados[0],sizeof(fann_type)*i*num_caracteristicas);
	count = i;
	while(count--){
		caracteristicas = dados[count];
		out[count] = caracteristicas;

		in[count] =  caracteristicas + offset;
	}

	dados_aleatorios->set_train_data(i,num_caracteristicas-offset,in,offset,out);

	dados_aleatorios->save_train(train_file);
	dados_aleatorios->shuffle_train_data();

	//rede_neural->init_weights(*dados_aleatorios);
	if(cascade)
		rede_neural->cascadetrain_on_data(*dados_aleatorios, num_neuronios_invisiveis, 1,0);
	else
		rede_neural->train_on_data(*dados_aleatorios, turnos, turnos_por_amostra, margem_de_erro);

	printf("MSE Final: %0.10f\n",rede_neural->test_data(*dados_aleatorios));
	delete dados_aleatorios;

	rede_neural->save("/tmp/net");
	rede_neural->save(ann_file);

	return dados_analisados = dados_escalados;
}

fann_type **CCerebro::getDadosGeneralizados(){
	if(dados_generalizados)
		return dados_generalizados;

	if(!getDadosAnalisados()){
		printf("Sem dados generalizados.\n");
		return NULL;
	}
	fann_type *caracteristicas, *memoria;

	/* TERMINAR IMPLEMENTACAO QUE PERMITE ALTERNAR ENTRE CARACTERISTICAS IDENTIFICADAS OU NAO*/
	int u,count;
	dados_generalizados = (fann_type**)malloc(sizeof(fann_type)*num_dados);
	caracteristicas = (fann_type*)malloc(sizeof(fann_type)*num_dados*num_caracteristicas);
	fann_type **in, **out;

	count = num_dados;
	while(count--){
		dados_generalizados[count] = caracteristicas;
		memoria = rede_neural->run(dados_analisados[count] + offset);
		memcpy(caracteristicas,memoria,offset*sizeof(fann_type));

		u=num_caracteristicas;
		while(u--){
			switch(limites[u]){
				case LIMITE_ORIGINAL:///Sem calculos (linear)
					caracteristicas[u] = memoria[u];
					break;
				case LIMITE_RESCALADO:///Valor calculado a partir da razao com o maximo, entre -1 e 1
					caracteristicas[u] = (int)round(((memoria[u] + 1) / 2)  * maximos[u]);
					break;
				case LIMITE_NORMALIZADO:///Valor normalizado entre -1 e 1
					//caracteristicas[u] = (caracteristicas_originais[u] - meios[u]) / (diferencas[u] / 2);
					caracteristicas[u] = (int)round(((diferencas[u] / 2) * memoria[u]) + meios[u]);
					break;
				case LIMITE_DESVIO:
					//caracteristicas[u] = (caracteristicas_originais[u] - medias[u]) / desvios[u];
					caracteristicas[u] = (int)round((memoria[u] * desvios[u]) +  medias[u]);
					break;
				case LIMITE_NULO:
					caracteristicas[u] = 0;

				break;
			}
		}
		caracteristicas += num_caracteristicas;
		//print_entradas(caracteristicas-num_caracteristicas,num_caracteristicas);
	}
	return dados_generalizados;
}

fann_type **CCerebro::getDadosDeduzidos(){
	if(dados_deduzidos)
		return dados_deduzidos;

	if(!getDadosGeneralizados()){
		printf("Sem dados deduzidos.\n");
		return NULL;
	}

	fann_type *caracteristicas;

	dados_deduzidos = (fann_type**)malloc(sizeof(fann_type)*num_dados);
	caracteristicas = (fann_type*)malloc(sizeof(fann_type)*num_dados*num_caracteristicas);

	int count;
	count = num_dados;
	while(count--){
		dados_deduzidos[count] = caracteristicas;
		memcpy(caracteristicas,dados_generalizados[count],sizeof(fann_type)*num_caracteristicas);
		caracteristicas += num_caracteristicas;
	}

	CCaracteristicas *entradas,*entradas_comparadas,*tmp;
	int num_entradas_unicas = 0;
	count = num_dados;
	entradas = NULL;
	while(count--){
		entradas_comparadas = new CCaracteristicas(num_caracteristicas, dados_deduzidos[count], num_caracteristicas, dados_generalizados[count]);
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
	//dados_deduzidos = dados_generalizados;

	printf("%d padrões encontrados.\n", num_entradas_unicas);

	return dados_deduzidos;
}

/*int CCortex::deduzCaracteristicas(int grupo){

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
	printf("%d padrões encontrados.\n", num_entradas_unicas);

	free(in[num_dados-1]);
	free(in);

	return 1;
}
*/
