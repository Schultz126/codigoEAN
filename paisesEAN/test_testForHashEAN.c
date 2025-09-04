#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "PAISES_EANreader.c"

// Forward declarations from testForHashEAN.c
#define TABLE_SIZE 128
typedef struct {
    Country country;
    int *next;
} HashNode;

extern HashNode *hashTable[TABLE_SIZE];
extern bool hashInitialize();
extern bool hashInsert(Country *country);
extern HashNode *hashSearch(Country *country);

// Helper to create a Country struct
Country makeCountry(int code, const char *name) {
    Country c;
    c.EANcode = code;
    strncpy(c.country, name, sizeof(c.country));
    c.country[sizeof(c.country)-1] = '\0';
    return c;
}

void test_hashInsert_and_hashSearch() {
    hashInitialize();

    Country c1 = makeCountry(123, "Testland");
    Country c2 = makeCountry(456, "Examplestan");
    Country c3 = makeCountry(789, "Mockovia");

    assert(hashInsert(&c1) == true);
    assert(hashInsert(&c2) == true);
    assert(hashInsert(&c3) == true);

    HashNode *found1 = hashSearch(&c1);
    assert(found1 != NULL);
    assert(found1->country.EANcode == 123);
    assert(strcmp(found1->country.country, "Testland") == 0);

    HashNode *found2 = hashSearch(&c2);
    assert(found2 != NULL);
    assert(found2->country.EANcode == 456);
    assert(strcmp(found2->country.country, "Examplestan") == 0);

    HashNode *found3 = hashSearch(&c3);
    assert(found3 != NULL);
    assert(found3->country.EANcode == 789);
    assert(strcmp(found3->country.country, "Mockovia") == 0);

    // Search for a non-inserted country
    Country c4 = makeCountry(999, "Nowhere");
    assert(hashSearch(&c4) == NULL);

    printf("All hashInsert and hashSearch tests passed!\n");
}

int main() {
    test_hashInsert_and_hashSearch();
    return 0;
}