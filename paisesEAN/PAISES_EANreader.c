#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#define MAX_CHARACTERS 128
//#define DEBUG

typedef struct {
    char country[MAX_CHARACTERS];
    char code[3];
    int EANcode;
} Country;

void readerPaisesEAN(Country *destiny) {
    FILE *i;
    if(!(i = fopen("PAISES_EAN.txt", "r+"))) {
        perror("ERRO, abertura PAISES_EAN.txt");
        return;
    }
    int index = 0;
    char aux[MAX_CHARACTERS];
    // Lê cada informação do txt e passa para as variáveis tipo char do struct
    while(fgets(aux, MAX_CHARACTERS, i) != NULL) {
        sscanf(aux, "%[^,],%[^\n]", destiny[index].code, destiny[index].country);
        // Converte a variável auxiliar code em um inteiro utilizável
        destiny[index].EANcode = atoi(destiny[index].code);
        #ifdef DEBUG
            printf("Código: %s, País(es): %s\n", destiny[index].code, destiny[index].country);
        #endif
        index++;
    }
    fclose(i);
}
/*
int main() {
    Country countries[1024];
    readerPaisesEAN(countries);

    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
}*/
