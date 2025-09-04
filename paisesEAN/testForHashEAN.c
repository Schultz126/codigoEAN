#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include "PAISES_EANreader.c"

#define HASH 61
#define TABLE_SIZE 128
#define INSERT_MAX 100
#define ALL_COUNTRIES 534
#define DEBUG  

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

unsigned int hash(Country* country) {
    return country->EANcode % HASH;
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
    int index = hash(country);  
    if(hashTable[index] != NULL) {
        // Retorna falso neste primeiro momento
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

void printTable() {
    printf("Início\n");
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(hashTable[i] == NULL) {
            printf("---> NULL\n");
        } else {
            printf("---> Código: %d, Nome: %s",hashTable[i]->country.EANcode, hashTable[i]->country.country);
        }
    }
    printf("Fim");
}

int main() {
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
                reset();
            } else {
                red();
                printf("Código: %d, Nome: %s\n Não foi inserido\n",toBeInserted[i].EANcode, toBeInserted[i].country);
                reset();
            }
        }
    #endif


    printTable();
}

