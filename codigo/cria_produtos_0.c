#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifndef QUANT
    #define QUANT 30
#endif

#define MAX 100

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[100];
    double valor;
    Data data;
} Produto;

typedef struct{
    Produto linha;
    char codigo_ean[14];
}Vendas;

void cria_data(int ano_inicial, int *dia, int *mes, int *ano)
{
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    *ano = rand() % 6 + ano_inicial;
    *mes = rand() % 12 + 1;
    if (*mes == 2 && ((*ano % 4) == 0 && ((*ano % 100) != 0 || (*ano % 400) == 0)))
        dias_mes[2] = 29;
    *dia = rand() % dias_mes[*mes] + 1;
}

// Função para gerar um código EAN de 13 dígitos aleatórios
void rand_ean(char *ean)
{
    for (int i = 0; i < 13; i++) {
        ean[i] = '0' + (rand() % 10);
    }
    ean[13] = '\0';
}

void main(int argc, char *argv[])
{
    FILE *i, *o;
    char produtos[MAX][100]; // Armazena os nomes dos produtos
    int iprod = 0, cont = 0, quant = QUANT;

    if ((i = fopen("nome_produtos.txt", "rt")) == NULL) {
        perror("ERRO abertura entrada");
        return;
    }
    if ((o = fopen("venda_produtos.txt", "wt")) == NULL) {
        perror("ERRO abertura saida");
        fclose(i);
        return;
    }

    // Lê todos os nomes de produtos para o array
    while (fgets(produtos[iprod], sizeof(produtos[iprod]), i) != NULL && iprod < MAX) {
        produtos[iprod][strcspn(produtos[iprod], "\n\r")] = 0; // Remove quebras de linha
        printf("%s\n", produtos[iprod]);
        iprod++;
    }
    fclose(i);

    if (iprod == 0) {
        fprintf(stderr, "Nenhum produto encontrado no arquivo.\n");
        fclose(o);
        return;
    }

    srand(time(NULL));
    if (argc > 1)
        quant = atoi(argv[1]);

    while (cont < quant) {
        Vendas venda_unica;
        int prod_aleatorio = rand() % iprod;

        cria_data(2020, &venda_unica.linha.data.dia, &venda_unica.linha.data.mes, &venda_unica.linha.data.ano);
        strcpy(venda_unica.linha.nome, produtos[prod_aleatorio]);
        venda_unica.linha.valor = (rand() % 9999) / 100.0;

        rand_ean(venda_unica.codigo_ean);

        fprintf(o, "%02d/%02d/%04d;%s;%s;%6.0f;%6.2f\n",
                venda_unica.linha.data.dia,
                venda_unica.linha.data.mes,
                venda_unica.linha.data.ano,
                venda_unica.codigo_ean,
                venda_unica.linha.nome,
                (double)(rand() % 999) / 100.0 + 1, // Quantidade
                venda_unica.linha.valor); // Valor total

        cont++;
    }
    fclose(o);
}
