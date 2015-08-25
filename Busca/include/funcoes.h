#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdio.h>
#include <string.h>

struct ponteiro_t {
	 struct ponteiro_t *n;
	 char *p;
	 int c;
	 size_t t;
};

 char *normaliza(char*, size_t);
 float LCS(char*, char*,int,int);
 float identica(char*, char*,int,int);
 float levenshtein(char*,char*,int,int);
		
#endif