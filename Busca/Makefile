#CPPFLAGS= -Wall -g -pg -march=athlon-xp -O2 -pipe
CPPFLAGS= -static -march=pentium4 -O2 -pipe -fomit-frame-pointer
CC=g++

FANN_LIBS=fann-2.1.0/src/.libs
FANN_LIB=libfann.a
FANN_INCLUDE=fann-2.1.0/src/include
TINY_INCLUDE=./
LDFLAGS= -I${TINY_INCLUDE} -I${FANN_INCLUDE} -L${FANN_LIBS} -lfann 

all: Busca
TINYXML=tinyxml.o tinyxmlparser.o tinyxmlerror.o tinystr.o

Busca: Griffon.o Cerebro.o \
DocumentoIA.o ArquivoXML.o Documento.o \
PaginaXML.o Pagina.o Bloco.o \
LinhaXML.o Linha.o Palavra.o Texto.o Letra.o \
Caracteristicas.o funcoes.o ${TINYXML} ${FANN_LIBS}/${FANN_LIB}

clean: 
		rm -rf *.o Busca dump tmp callgrind* massif*

dump: dump.o
	${CC} ${CPPFLAGS} dump.o -o dump	
# DO NOT DELETE
