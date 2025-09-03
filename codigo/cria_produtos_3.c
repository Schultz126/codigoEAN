#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "criadorArquivoAux.c"

#ifndef QUANT
	#define QUANT 30
#endif

//#define DEBUG

#define MAX 100

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[MAX]; // Nome do produto
    double valor;
    int quantidade;
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
    char auxLine[256];
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
    while(index < MAX && fgets(auxLine, MAX, i) != NULL) {
        sscanf(auxLine, "%[^,],%s", auxProd[index].linha.nome, auxProd[index].codigo_ean);
        #ifdef DEBUG
        printf("%s, %s\n", auxProd[index].linha.nome, auxProd[index].codigo_ean); 
        #endif
        index++;
    }
    /*
        Código para escrever todas as informações no arquivo principal
        tendo em vista que os produtos devem estar randomizados
    */

    for(int index = 0; index < MAX; index++) {
        int randomIndex = rand() % MAX;
        cria_data(2020, &produto.linha.data.dia, &produto.linha.data.mes, &produto.linha.data.ano);
        fprintf(o, "%02d/%02d/%02d %s, %s %d %.2f\n", produto.linha.data.dia, 
            produto.linha.data.mes,
            produto.linha.data.ano,
            auxProd[randomIndex].linha.nome,
            auxProd[randomIndex].codigo_ean,
            rand() % 999, // Quantidade (int)
            (double)(rand() % 10000) / 100.0 // Valor (double, 0.00 to 99.99)
        );
    }
 
    fclose(i);
    fclose(o);
}

void createProduct(Vendas *produto) {
    FILE *i;
    char auxLine[MAX];
    if(!(i = fopen("C:/Users/rafae/OneDrive/Documentos/2 Ano/Estrutura de Dados/codigoEAN/arquivosTexto/info_produtos.txt", "r"))) {
        perror("ERRO, abertura de info_produtos.txt para leitura");
        return;
    }

    int index = 0;
    while(index < MAX && fgets(auxLine, 256, i) != NULL) {
        sscanf(auxLine, "%2d/%2d/%4d %[^,], %s %d %lf",
            &produto[index].linha.data.dia,
            &produto[index].linha.data.mes,
            &produto[index].linha.data.ano,
            produto[index].linha.nome,
            produto[index].codigo_ean,
            &produto[index].linha.quantidade,
            &produto[index].linha.valor);

        index++;
    }
    fclose(i);
}

int main() {
    Vendas produto[MAX];

    createAuxFile();
    createMainFile();
    createProduct(produto);

    #ifdef DEBUG
    int index = 0;
    while(index < MAX) {
        printf("%02d/%02d/%04d, %s, %s, %d, %.2f\n", produto[index].linha.data.dia,
            produto[index].linha.data.mes,
            produto[index].linha.data.ano,
            produto[index].linha.nome,
            produto[index].codigo_ean,
            produto[index].linha.quantidade,
            produto[index].linha.valor);

            index++;
    }
    #endif
}