#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

//#define DEBUG
#define ITENS 101
#define MAX_CHARS 50
#define EAN_QUANT 14

void leitorDeArquivo(FILE *arquivoParaLer, char variavelDestino[][MAX_CHARS]);
void criarCodigoEANrand(char codigos[][EAN_QUANT]);

void createAuxFile() {
    FILE *i, *o; 
    char produtos[ITENS][MAX_CHARS], codigos[ITENS][EAN_QUANT];
    char line[MAX_CHARS + EAN_QUANT + 3];
     if(!(i = fopen("C:/Users/rafae/OneDrive/Documentos/2 Ano/Estrutura de Dados/codigoEAN/arquivosTexto/nome_produtos.txt", "r"))) {
        perror("ERRO, abertura nome_produtos.txt");
        return;
     } 
     #ifdef DEBUG
     else {
        printf("Td certo");
     }
     #endif
     if(!(o = fopen("C:/Users/rafae/OneDrive/Documentos/2 Ano/Estrutura de Dados/codigoEAN/arquivosTexto/nome_produtos_com_codigoEAN.txt", "w+"))) {
        perror("ERRO, criação nome_produtos_com_codigo.txt");
        return;
     } 
     #ifdef DEBUG
     else {
        printf("\nTd certo");
     }
     #endif

     leitorDeArquivo(i, produtos);
     criarCodigoEANrand(codigos);

     for(int index = 0; index < ITENS; index++) {
        sprintf(line, "%s, %s", produtos[index], codigos[index]);
        #ifdef DEBUG
        printf("%s\n", line);
        #endif
        fprintf(o, "%s\n", line);
     }

     fclose(i);
     fclose(o);
}

void leitorDeArquivo(FILE *arquivoParaLer, char variavelDestino[][MAX_CHARS]) {
    // Verificação de erros
    if (arquivoParaLer == NULL || variavelDestino == NULL) {
        return;
    }

    int index = 0;
    srand(time(NULL));
    while (fgets(variavelDestino[index], MAX_CHARS, arquivoParaLer) != NULL) {
        size_t len = strlen(variavelDestino[index]);
        if (len > 0 && variavelDestino[index][len - 1] == '\n') {
            // Adiciona o null terminator no final da string
            variavelDestino[index][len - 1] = '\0';
        }

        index++;

        if (index >= ITENS) {
            break;
        }
    }
    #ifdef DEBUG
     for(int i = 0; i < ITENS; i++) {
      printf("%s\n", variavelDestino[i]);
     }
     #endif
}

void criarCodigoEANrand(char codigos[][EAN_QUANT]) {
   for (int i = 0; i < ITENS; i++) {
        // Gera um código EAN de 13 dígitos aleatórios.
        for (int j = 0; j < 13; j++) {
            codigos[i][j] = '0' + (rand() % 10);
        }
        codigos[i][13] = '\0';
        #ifdef DEBUG
        printf("%s\n", codigos[i]);
        #endif
    }
}
