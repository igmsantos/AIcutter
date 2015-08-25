#include "include/funcoes.h"
#include <stdexcept>
#include <time.h>

char *normaliza(char *texto, size_t tamanho){
	static 	char *tabela_normalizada = NULL;
	size_t count;

	if(tabela_normalizada == NULL){
		char in[]    = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏĞÑÒÓÔÕÖ×ØÙÚÛÜİŞßàáâãäåæçèéêëìíîïğñòóôõöùúûü";//ışÿ?¼½|·º¹²³°ª¸­??¿,.;:<=>+_-%$¨^~*#&@/\\\"`'{}[]()§ ";
		char out[]   = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZAAAAAAACEEEEIIIIDNOOOOOX0UUUUYBBAAAAAAACEEEEIIIIONOOOOOUUUU";//YBY";
		size_t len = strlen(in);
		//char out[len];
		//out = new char[len];
		//memset(out,' ',len);
	    //memcpy(out,mask,strlen(mask));
		tabela_normalizada = new char [0x100];
		memset(tabela_normalizada,' ',0x100);
	   for(count=0;count < len;count++){
			//printf("%c(%d)\t%c(%d)\n", (unsigned char)in[count],(unsigned char)in[count],
				//						out[count],out[count]);
			tabela_normalizada[(unsigned char)in[count]] = out[count];
		}
		for(count=0;count<=0xFF;count++){
			//printf("%c %d\n",tabela_normalizada[count],tabela_normalizada[count]);
					//printf("%u\n", count);
		}
		
	}

	for(count=0;count < tamanho;count++)
	     texto[count] = tabela_normalizada[(unsigned char)texto[count]]; //Traduz caracter

	return texto;
}

float identica(char *s1, char *s2, int m, int n){
	if(m == n)	
		return(!memcmp(s1,s2,m));
	return 0;
}
float regexp(char*s1, char *s2, int m, int n){
	return 0.0;
}
float LCS(char *s1, char *s2, int m, int n){
	static char tabela_LCS[64][64];
	int mp = m+1;
	int np = n+1;
	int i;
	int j;
	for(i=1; i < mp; i++) tabela_LCS[i][0]=0;
	for(j=0; j < np; j++) tabela_LCS[0][j]=0;
	
	for (i=1; i <= m; i++) {
		for (j=1; j <= n; j++) {
			if (s1[i-1]==s2[j-1])
				tabela_LCS[i][j] = tabela_LCS[i-1][j-1] + 1;
			else if (tabela_LCS[i-1][j] >= tabela_LCS[i][j-1])
				tabela_LCS[i][j] = tabela_LCS[i-1][j];
			else
				tabela_LCS[i][j] = tabela_LCS[i][j-1];
		}
	}
	return (float)tabela_LCS[m][n] / ((m+n)>>1);	
}	
float levenshtein(char *s,char *t,int n,int m)
{
  static int tabela_levenshtein[sizeof(int)*64*64];
  int a,b,c,d,e,i,j,cost;
  if(n!=0&&m!=0)
  {
    //tabela_levenshtein=(int*)malloc((sizeof(int))*(m+1)*(n+1));
    m++;
    n++;

	for(d=0;d<n;d++)
		tabela_levenshtein[d]=d;
    for(d=0;d<m;d++)
		tabela_levenshtein[d*n]=d;
	
    for(i=1;i<n;i++)
      for(j=1;j<m;j++)
	  {
        if(s[i-1]==t[j-1])
          cost=0;
        else
          cost=1;

		//d = (j-1)*n+i;	
		e = j*n+i;
		a = tabela_levenshtein[(j-1)*n+i]+1;
		//a = tabela_levenshtein[d]+1;
		b = tabela_levenshtein[e-1]+1;
		c = tabela_levenshtein[(j-1)*n+i-1]+cost;
		//c = tabela_levenshtein[d-1]+cost;

	  if(b<a)
		a=b;
	  if(c<a)
		a=c;

        tabela_levenshtein[e]=a;
      }

    d = tabela_levenshtein[n*m-1];

	if(d <= ((n+m)>>1))
		return (float)(d / ((n+m)>>1));
    return 0;
  }
  else 
    return -1;
}
