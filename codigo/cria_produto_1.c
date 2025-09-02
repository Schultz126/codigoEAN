#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Definir a estrutura para armazenar os dados de um produto.
typedef struct {
    char codigo_ean[14]; // EAN13 + 1 para o caractere nulo
    char nome[100];      // Nome do produto
    double valor;        // Valor do produto
} Produto;

int main() {
    // Declara e inicializa uma lista (array) de produtos.
    // Você pode adicionar mais produtos aqui se quiser.
    Produto lista_produtos[] = {
        {"7891000100125", "Arroz Tio Joao 5kg", 25.50},
        {"7892000200236", "Feijao Camil 1kg", 8.90},
        {"7893000300347", "Oleo de Soja Liza 900ml", 6.75},
        {"7894000400458", "Acucar Cristal 2kg", 7.20}
    };
    // Calcula o número de produtos na lista.
    // Isso é feito dividindo o tamanho total do array pelo tamanho de um único elemento.
    int numero_de_produtos = sizeof(lista_produtos) / sizeof(lista_produtos[0]);
    printf("--- Lista de Produtos ---\n\n");
    // Loop para iterar sobre a lista e exibir os dados de cada produto.
    for (int i = 0; i < numero_de_produtos; i++) {
        printf("Produto %d:\n", i + 1);
        printf("  Codigo EAN: %s\n", lista_produtos[i].codigo_ean);
        printf("  Nome: %s\n", lista_produtos[i].nome);
        printf("  Valor: R$ %.2f\n", lista_produtos[i].valor);
        printf("\n"); // Adiciona uma linha em branco para melhor visualizacao
    }
    printf("-------------------------\n");
    return 0;
}
