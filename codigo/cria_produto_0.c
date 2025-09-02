#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifndef QUANT
	#define QUANT 30
#endif

#define MAX 100


typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[100];      // Nome do produto
    double valor;
    Data data;
} Produto;

typedef struct{
    Produto linha;
    char codigo_ean[14];    // EAN13 + 1 para o caractere nulo
}Vendas;

void cria_data( int ano_inicial, int *dia, int *mes, int *ano )
{                     //   1   2   3   4   5   6   7   8   9  10  11  12
	int dias_mes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	*ano = rand()%6+ano_inicial;
	*mes = rand()%12+1;
	if( *mes == 2 && ((*ano % 4)==0 && ((*ano % 100)!=0 || (*ano % 400)==0)))
		dias_mes [ 2 ] = 29;
	*dia = rand()%dias_mes[ *mes ]+1;
}

void main(int argc, char *argv[])
{
	FILE *i, *o;
	Vendas vendas[MAX];
    /*char prod[150][50];*/
	int iprod=0, cont = 0, quant = QUANT;
	/*char linha[ 200 ];*/

	if( (i = fopen( "nome_produtos.txt", "rt") ) == NULL )
		perror("ERRO abertura entrada\n");
	if( (o = fopen( "venda_produtos.txt", "wt")) == NULL )
		perror( "ERRO abertura saida\n");
	while( ! feof( i ) ) {
		fgets( vendas[iprod].linha.nome, sizeof(vendas.linha.nome), i );
		vendas[ iprod ].linha.nome[ strlen(vendas[iprod].linha.nome)-1 ] = 0;
		printf( "%s\r ", vendas[ iprod ].linha.nome );
		iprod++;
	}
	fclose( i );
	srand( time( NULL ) );
	if( argc > 1 )
		quant = atoi( argv[ 1 ] );
	while( cont < quant ) {
    for (int i = 0; i < MAX; i++) {
        // Gera um código EAN de 13 dígitos aleatórios.
        for (int j = 0; j < 13; j++) {
            vendas[i].codigo_ean[j] = '0' + (rand() % 100);
        }
    }
		cria_data( 2020, &vendas.linha.data.dia, &vendas.linha.data.mes, &vendas.linha.data.ano );
		sprintf( vendas.linha, "%02d/%02d/%04d; %13d; %s; %6.0f; %6.2f\r", vendas.linha.data.dia, vendas.linha.data.mes,
		            vendas.linha.data.ano, vendas.codigo_ean, vendas.linha.nome ,(rand()%999)/100.0+1, (rand()%9999)/100.0 );
		printf( "%s\r ", linha );
		fprintf(o, linha );
		cont++;
	}
	fclose( i );
	fclose( o );
}
