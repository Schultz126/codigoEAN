#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "PAISES_EANreader.c"

typedef struct {
    Country country;

};

#define HASH 101

unsigned int hash(Country* country) {
    return country->EANcode % HASH;
}

void initializeHashTable() {

}

