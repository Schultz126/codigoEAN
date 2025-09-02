#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "criadorArquivoAux.c"

#ifndef QUANT
	#define QUANT 30
#endif

#define DEBUG

#define MAX 100

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[MAX];      // Nome do produto
    double valor;
    Data data;
} Produto;

typedef struct{
    Produto linha;
    char codigo_ean[14];    // EAN13 + 1 para o caractere nulo
}Vendas;

void cria_data( int ano_inicial, int *dia, int *mes, int *ano )
{                     //   1   2   3   4   5   6   7   8   9  10  11  12
	int dias_mes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	*ano = rand()%6+ano_inicial;
	*mes = rand()%12+1;
	if( *mes == 2 && ((*ano % 4)==0 && ((*ano % 100)!=0 || (*ano % 400)==0)))
		dias_mes [ 2 ] = 29;
	*dia = rand()%dias_mes[ *mes ]+1;
}

void createMainFile() {
    FILE *i, *o;
    Vendas produto;
    Vendas auxProd[MAX];
    char auxLine[MAX];
    if(!(i = fopen("C:/Users/rafae/OneDrive/Documentos/2 Ano/Estrutura de Dados/codigoEAN/arquivosTexto/nome_produtos_com_codigoEAN.txt", "r"))) {
        perror("ERRO, abertura nome_produtos_com_codigoEAN.txt");
        return;
    }
    #ifdef DEBUG
    printf("td certo\n");
    #endif
    if(!(o = fopen("C:/Users/rafae/OneDrive/Documentos/2 Ano/Estrutura de Dados/codigoEAN/arquivosTexto/info_produtos.txt", "w+"))) {
        perror("ERRO, abertura info_produtos.txt");
        return;
    }
    #ifdef DEBUG
    printf("td certo\n");
    #endif

    int index = 0;
    // Lê o arquivo auxiliar com nomes associados a códigos
    while(fgets(auxLine, MAX, i) != NULL) {
        sscanf(auxLine, "%[^,],%s", auxProd[index].linha.nome, auxProd[index].codigo_ean);
        #ifdef DEBUG
        printf("%s, %s\n", &auxProd[index].linha.nome, &auxProd[index].codigo_ean); 
        #endif
        index++;
    }
    /*
        Código para escrever todas as informações no arquivo principal
        tendo em vista que os produtos devem estar randomizados
    */
 
    fclose(i);
    fclose(o);
}

int main() {
    createAuxFile();
    createMainFile();
}