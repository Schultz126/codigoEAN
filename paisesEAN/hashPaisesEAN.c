#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "PAISES_EANreader.c"

#define HASH 37
#define INSERT_MAX 30
#define ALL_COUNTRIES 534
#define TABLE_SIZE 50
//#define DEBUG  
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

typedef struct {
    HashNode **table;
    int numberOfElements;
    float loadFactor;
    int size;
}HashTable;

HashTable *hashTable;

unsigned int hash(int code) {
    return code % HASH;
}

static inline int hashDuplo(Country* country) {
    return 1 + (country->EANcode % (hashTable->size - 2));
}

bool hashInitialize() {
    for(int i = 0; i < hashTable->size; i++) {
        hashTable->table[i] = NULL;
    }
}

bool hashInsert(Country *country) {
    if(country == NULL) {
        return false;
    }
    int index = hash(country->EANcode);  
    if(hashTable->table[index] != NULL) {
        
        // Endereçamento aberto: procura próxima posição livre
        int originalIndex = index;
        int i = 1;
        do {
            index = (originalIndex + (i*i)) % hashTable->size;
            if (hashTable->table[index] == NULL) {
            hashTable->table[index] = (HashNode*)malloc(sizeof(HashNode));
            if (hashTable->table[index] == NULL) {
                return false;
            }
            hashTable->table[index]->country = *country;
            hashTable->table[index]->next = NULL;
            hashTable->numberOfElements++;
            hashTable->loadFactor = ((float)(hashTable->numberOfElements) / (hashTable->size));
            return true;
            }
            i++;
        } while (index != originalIndex && i < hashTable->size);
        // Tabela cheia
        return false;
    }
    hashTable->table[index] = (HashNode*)malloc(sizeof(HashNode));
    if (hashTable->table[index] == NULL) {
        return false;
    }
    hashTable->table[index]->country = *country;
    hashTable->table[index]->next = NULL;
    hashTable->numberOfElements++;
    hashTable->loadFactor = ((float)(hashTable->numberOfElements) / (hashTable->size));
    return true;
}

int hashSearch(int EANcode) {
    int index = hash(EANcode);  
    int originalIndex = index;
    int i = 0;
    while (i < TABLE_SIZE) {
        if (hashTable->table[index] != NULL) {
            if (hashTable->table[index]->country.EANcode == EANcode) {
                comparacoes++;
                return index;
            }
        } else {
            // If we hit a NULL, the item is not in the table
            comparacoes = 0;
            return -1;
        }
        i++;
        comparacoes++;
        // Função para cálculo do novo index deve ser comum entre inserção e busca
        index = (originalIndex + (i*i)) % hashTable->size;
    }
    return -1;
}

bool deleteFromHashTable(HashNode *toDelete) {
    int index = hashSearch(toDelete->country.EANcode);
    if(index < 0) {
        return false;
    }
    free(toDelete);
    hashTable->table[index] = NULL;
    hashTable->numberOfElements--;
    hashTable->loadFactor = ((float)hashTable->numberOfElements / hashTable->size);
    return true;
    
}

void printTable() {
    printf("Início\n");
    for(int i = 0; i < TABLE_SIZE; i++) {
        if(hashTable->table[i] == NULL) {
            printf("---> NULL\n");
        } else {
            printf("---> Código: %d, Nome: %s\n",hashTable->table[i]->country.EANcode, hashTable->table[i]->country.country);
        }
    }
    printf("Fator de carga: %.3f\n", hashTable->loadFactor);
    printf("Fim");
}

// A corrected and complete rehash function would look something like this:
bool rehash(HashTable *table) {
    size_t novoTamanho = 2 * table->size;
    HashNode **novaTabela = calloc(novoTamanho, sizeof(HashNode*)); // Use double pointer
    if (!novaTabela) {
        return false;
    }
    
    HashNode **tabelaAntiga = table->table;
    size_t capacidadeAntiga = table->size;
    
    // Update the hash table struct to point to the new table
    table->table = novaTabela;
    table->numberOfElements = 0;
    table->size = novoTamanho;

    // Iterate through the old table and re-insert elements
    for (size_t i = 0; i < capacidadeAntiga; i++) {
        if (tabelaAntiga[i] != NULL) {
            hashInsert(&tabelaAntiga[i]->country); 
            free(tabelaAntiga[i]); // Free the old node
        }
    }
    free(tabelaAntiga); // Free the old table array
    return true;
}

int main() {
    int inseridos = 0, naoInseridos = 0;
    Country countries[ALL_COUNTRIES], toBeInserted[INSERT_MAX];
    readerPaisesEAN(countries);

    // Allocate memory for the hashTable struct
    hashTable = (HashTable*)malloc(sizeof(HashTable));
    if (hashTable == NULL) {
        // Handle allocation error
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
            printf("Código: %d, Nome: %s Inserido com sucesso\n",toBeInserted[i].EANcode, toBeInserted[i].country);
            inseridos++;
            reset(); 
        } else {
            red();
            printf("Código: %d, Nome: %s Não foi inserido\n",toBeInserted[i].EANcode, toBeInserted[i].country);
            naoInseridos++;
            reset();
        }
    }
    
    printTable();

    int searched = hashSearch(790);

    if(searched == -1) {
        printf("\nCódigo 790 não encontrado\n");
    } else {
        printf("\nCódigo: %d, Nome: %s\n",hashTable->table[searched]->country.EANcode, hashTable->table[searched]->country.country);
    }
    printf("Comparações feitas durante a busca: %d\n",comparacoes);
    printf("Foram inseridos: %d países e %d ficaram de fora\n", inseridos, naoInseridos);

    printf("Nó deletado: %d %s\n",toBeInserted[10].EANcode, toBeInserted[10].country);
    int indexToDelete = hashSearch(toBeInserted[10].EANcode);
    deleteFromHashTable(hashTable->table[indexToDelete]);
    printTable();
}

