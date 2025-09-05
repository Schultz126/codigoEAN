#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "PAISES_EANreader.c"

#define HASH 37
#define INSERT_MAX 120
#define ALL_COUNTRIES 534
#define TABLE_SIZE 50
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
    bool isDeleted; // flag de deleção para facilitar a função de remoção
} HashNode;

typedef struct {
    HashNode **table; // Ponteiro duplo para poder realocar o vetor
    int numberOfElements;
    float loadFactor;
    int size;
} HashTable;

HashTable *hashTable;

bool rehash(HashTable *table);

unsigned int hash(int code) {
    return code % HASH;
}

bool hashInitialize() {
    for(int i = 0; i < hashTable->size; i++) {
        hashTable->table[i] = NULL;
    }
    return true;
}

bool hashInsert(Country *country) {
    if(country == NULL) {
        return false;
    }

    if (hashTable->loadFactor >= 0.6) {
        if (!rehash(hashTable)) {
            return false; // Rehash failed
        }
    }

    int originalIndex = hash(country->EANcode);
    int index = originalIndex;
    int i = 0;

    do {
        if (i > 0) {
            index = (originalIndex + (i*i)) % hashTable->size;
        }

        // Procura um espaço livre ou um item marcado como deletado
        if (hashTable->table[index] == NULL || hashTable->table[index]->isDeleted) {
            if (hashTable->table[index] == NULL) {
                hashTable->table[index] = (HashNode*)malloc(sizeof(HashNode));
                if (hashTable->table[index] == NULL) {
                    return false;
                }
            }
            hashTable->table[index]->country = *country;
            hashTable->table[index]->isDeleted = false; // Not deleted anymore
            hashTable->numberOfElements++;
            hashTable->loadFactor = ((float)hashTable->numberOfElements / hashTable->size);
            return true;
        }

        i++;
    } while (i < hashTable->size);

    return false; // tabela cheia
}

int hashSearch(int EANcode) {
    int originalIndex = hash(EANcode);
    int index = originalIndex;
    int i = 0;
    comparacoes = 0;

    do {
        if (i > 0) {
            index = (originalIndex + (i*i)) % hashTable->size;
        }

        if (hashTable->table[index] == NULL) {
            return -1; // Not found
        }
        
        comparacoes++;

        if (!hashTable->table[index]->isDeleted && hashTable->table[index]->country.EANcode == EANcode) {
            return index; // Found
        }

        i++;
    } while (i < hashTable->size);

    return -1; // Not found after full search
}

bool deleteFromHashTable(int EANcode) {
    int index = hashSearch(EANcode);
    if(index < 0) {
        return false;
    }
    
    hashTable->table[index]->isDeleted = true;
    hashTable->numberOfElements--;
    hashTable->loadFactor = ((float)hashTable->numberOfElements / hashTable->size);
    return true;
}

void printTable() {
    printf("Início\n");
    for(int i = 0; i < hashTable->size; i++) {
        if(hashTable->table[i] == NULL) {
            printf("---> NULL\n");
        } else if (hashTable->table[i]->isDeleted) {
            printf("---> DELETED\n");
        } else {
            printf("---> Código: %d, Nome: %s\n", hashTable->table[i]->country.EANcode, hashTable->table[i]->country.country);
        }
    }
    printf("Fator de carga: %.3f\n", hashTable->loadFactor);
    printf("Fim\n");
}

bool rehash(HashTable *table) {
    size_t newSize = table->size * 2;
    HashNode **newTable = (HashNode**)calloc(newSize, sizeof(HashNode*));
    if (!newTable) {
        return false;
    }
    
    HashNode **oldTable = table->table;
    size_t oldSize = table->size;
    
    table->table = newTable;
    table->size = newSize;
    table->numberOfElements = 0; // Will be incremented by hashInsert

    for (size_t i = 0; i < oldSize; i++) {
        if (oldTable[i] != NULL && !oldTable[i]->isDeleted) {
            hashInsert(&oldTable[i]->country);
        }
        free(oldTable[i]);
    }
    free(oldTable);
    return true;
}

int main() {
    int inseridos = 0, naoInseridos = 0;
    Country countries[ALL_COUNTRIES], toBeInserted[INSERT_MAX];
    readerPaisesEAN(countries);

    hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        perror("Failed to allocate memory for hash table");
        return 1;
    }

    hashTable->table = (HashNode**)calloc(TABLE_SIZE, sizeof(HashNode*));
    if (hashTable->table == NULL) {
        perror("Failed to allocate memory for hash table array");
        free(hashTable);
        return 1;
    }
    
    hashTable->numberOfElements = 0;
    hashTable->size = TABLE_SIZE;
    hashInitialize();

    srand(time(NULL));
    for(int i = 0; i < INSERT_MAX; i++) {
        toBeInserted[i] = countries[rand() % ALL_COUNTRIES];
    }

    for(int i = 0; i < INSERT_MAX; i++) {
        if(hashInsert(&toBeInserted[i])) {
            green();
            printf("Código: %d, Nome: %s Inserido com sucesso\n", toBeInserted[i].EANcode, toBeInserted[i].country);
            inseridos++;
            reset(); 
        } else {
            red();
            printf("Código: %d, Nome: %s Não foi inserido\n", toBeInserted[i].EANcode, toBeInserted[i].country);
            naoInseridos++;
            reset();
        }
    }
    
    printTable();

    int searchCode = toBeInserted[10].EANcode;
    int searched = hashSearch(searchCode);

    if(searched == -1) {
        printf("\nCódigo %d não encontrado\n", searchCode);
    } else {
        printf("\nCódigo: %d, Nome: %s\n", hashTable->table[searched]->country.EANcode, hashTable->table[searched]->country.country);
    }
    printf("Comparações feitas durante a busca: %d\n", comparacoes);
    printf("Foram inseridos: %d países e %d ficaram de fora\n", inseridos, naoInseridos);

    printf("Nó a ser deletado: %d %s\n", searchCode, hashTable->table[searched]->country.country);
    if(deleteFromHashTable(searchCode)) {
        green();
        printf("Deleção do Código %d bem-sucedida.\n", searchCode);
        reset();
    } else {
        red();
        printf("Falha na deleção do Código %d.\n", searchCode);
        reset();
    }

    printf("\nBuscando o código deletado:\n");
    searched = hashSearch(searchCode);
    if(searched == -1) {
        green();
        printf("Código %d não encontrado (correto).\n", searchCode);
        reset();
    } else {
        red();
        printf("Código %d encontrado (INCORRETO).\n", searchCode);
        reset();
    }

    printTable();
    
    // Proper cleanup at the end of main
    for(int i = 0; i < hashTable->size; i++) {
        if(hashTable->table[i] != NULL) {
            free(hashTable->table[i]);
        }
    }
    free(hashTable->table);
    free(hashTable);
    
    return 0;
}