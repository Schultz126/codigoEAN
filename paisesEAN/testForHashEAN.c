#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "PAISES_EANreader.c"

#define HASH 101
#define TABLE_SIZE 128
#define INSERT_MAX 100
#define ALL_COUNTRIES 534
#define DEBUG  

int comparacoes = 0;

void red() {
    printf("\033[1;31m");
}
void green() {
    printf("\033[1;32m");
}
void reset() {
    printf("\033[m");
}

typedef struct {
    Country country;
    int *next;
}HashNode;

HashNode *hashTable[TABLE_SIZE];

unsigned int hash(int code) {
    return code % HASH;
}

static inline int hashDuplo(Country* country) {
    return 1 + (country->EANcode % (TABLE_SIZE - 2));
}

bool hashInitialize() {
    for(int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

bool hashInsert(Country *country) {
    if(country == NULL) {
        return false;
    }
    int index = hash(country->EANcode);  
    if(hashTable[index] != NULL) {
        
        // Endereçamento aberto: procura próxima posição livre
        int originalIndex = index;
        int i = 1;
        do {
            index = (originalIndex + (i*i)) % TABLE_SIZE;
            if (hashTable[index] == NULL) {
            hashTable[index] = (HashNode*)malloc(sizeof(HashNode));
            if (hashTable[index] == NULL) {
                return false;
            }
            hashTable[index]->country = *country;
            hashTable[index]->next = NULL;
            return true;
            }
            i++;
        } while (index != originalIndex && i < TABLE_SIZE);
        // Tabela cheia
        return false;
        return false;
        

    }
    hashTable[index] = (HashNode*)malloc(sizeof(HashNode));
    if (hashTable[index] == NULL) {
        return false;
    }
    hashTable[index]->country = *country;
    hashTable[index]->next = NULL;
    return true;
}

HashNode *hashSearch(int EANcode) {
    int index = hash(EANcode);  
    int originalIndex = index;
    int i = 0;
    while (i < TABLE_SIZE) {
        if (hashTable[index] != NULL) {
            if (hashTable[index]->country.EANcode == EANcode) {
                comparacoes++;
                return hashTable[index];
            }
        } else {
            // If we hit a NULL, the item is not in the table
            comparacoes = 0;
            return NULL;
        }
        i++;
        comparacoes++;
        // Função para cálculo do novo index deve ser comum entre inserção e busca
        index = (originalIndex + (i*i)) % TABLE_SIZE;
    }
    return NULL;
}

void printTable() {
    printf("Início\n");
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(hashTable[i] == NULL) {
            printf("---> NULL\n");
        } else {
            printf("---> Código: %d, Nome: %s\n",hashTable[i]->country.EANcode, hashTable[i]->country.country);
        }
    }
    printf("Fim");
}

int main() {
    int inseridos = 0, naoInseridos = 0;
    Country countries[ALL_COUNTRIES], toBeInserted[INSERT_MAX];
    readerPaisesEAN(countries);
    hashInitialize();

    srand(time(NULL));
    for(int i = 0; i < INSERT_MAX; i++) {
        toBeInserted[i] = countries[rand() % ALL_COUNTRIES];
    }
    #ifdef DEBUG
        for(int i = 0; i < INSERT_MAX; i++) {
            if(hashInsert(&toBeInserted[i])) {
                green();
                printf("Código: %d, Nome: %s Inserido com sucesso\n",toBeInserted[i].EANcode, toBeInserted[i].country);
                inseridos++;
                reset(); 
            } else {
                red();
                printf("Código: %d, Nome: %s\n Não foi inserido\n",toBeInserted[i].EANcode, toBeInserted[i].country);
                naoInseridos++;
                reset();
            }
        }
    #endif
    
    printTable();

    HashNode *searched = hashSearch(790);

    if(searched == NULL) {
        printf("\nCódigo 790 não encontrado\n");
    } else {
        printf("\nCódigo: %d, Nome: %s\n",searched->country.EANcode, searched->country.country);
    }
    printf("Comparações feitas durante a busca: %d\n",comparacoes);
    printf("Foram inseridos: %d países e %d ficaram de fora\n", inseridos, naoInseridos);

}

