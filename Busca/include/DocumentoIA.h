#ifndef DOCUMENTO_IA_H
#define DOCUMENTO_IA_H

#include "Caracteristicas.h"
#include "ArquivoXML.h"
		
//using namespace FANN;

class CDocumentoIA : public CArquivoXML
{
public:
	//~CDocumentoIA();

	char ident_file[FILENAME_MAX];		///< Arquivo de dados do treinamento de identificação .ident
	char verif_file[FILENAME_MAX];
	CDocumentoIA(char*,char*);
	CDocumentoIA(char*);

	fann_type **caracteristicas_visuais;
	fann_type **caracteristicas_espaciais;
	fann_type **caracteristicas_estatisticas;
	fann_type **caracteristicas_classificatorias;
	fann_type **caracteristicas_classificatorias_externas;

	int num_caracteristicas_visuais;
	int num_caracteristicas_espaciais;
	int num_caracteristicas_estatisticas;
	int num_caracteristicas_classificatorias;

	fann_type **getCaracteristicasVisuais();
	fann_type **getCaracteristicasEspaciais();
	fann_type **getCaracteristicasEstatisticas();
	fann_type **getCaracteristicasClassificatorias();
	fann_type **getCaracteristicasExternasClassificatorias();
	int reproduzCaracteristicasClassificatorias(fann_type **, int);
	int getNumCaracteristicas();
	int getNumDados();
};
#endif
