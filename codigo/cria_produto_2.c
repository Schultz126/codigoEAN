#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Define a quantidade de produtos na lista.
// Você pode alterar este valor para criar mais ou menos produtos.
#define N 100
// Definir a estrutura para armazenar os dados de um produto.
typedef struct {
    char codigo_ean[14]; // EAN13 + 1 para o caractere nulo
    char nome[100];      // Nome do produto
    double valor;        // Valor do produto
} Produto;

int main() {
    // Inicializa a semente para a função rand() com o tempo atual.
    // Isso garante que os números gerados serão diferentes a cada execução.
    srand(time(NULL));
    // Declara uma lista (array) de N produtos.
    Produto lista_produtos[N];
    // Loop para preencher a lista com dados randômicos.
    for (int i = 0; i < N; i++) {
        // Gera um código EAN de 13 dígitos aleatórios.
        for (int j = 0; j < 13; j++) {
            lista_produtos[i].codigo_ean[j] = '0' + (rand() % 10);
        }
        lista_produtos[i].codigo_ean[13] = '\0'; // Adiciona o caractere nulo no final da string
        // Gera um nome de produto randômico.
        sprintf(lista_produtos[i].nome, "Produto Randomico %d", i + 1);
        // Gera um valor randômico entre 1.00 e 999.99.
        lista_produtos[i].valor = (double)(rand() % 10000) / 100.0;
    }
    printf("--- Lista de Produtos (Aleatoria) ---\n\n");
    // Loop para iterar sobre a lista e exibir os dados de cada produto.
    for (int i = 0; i < N; i++) {
        printf("Produto %d:\n", i + 1);
        printf("  Codigo EAN: %s\n", lista_produtos[i].codigo_ean);
        printf("  Nome: %s\n", lista_produtos[i].nome);
        printf("  Valor: R$ %.2f\n", lista_produtos[i].valor);
        printf("\n"); // Adiciona uma linha em branco para melhor visualizacao
    }
    printf("--------------------------------------\n");
    return 0;
}
