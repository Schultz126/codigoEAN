#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <time.h>
#include "cria_produtos_3.c"

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    Vendas produto;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

RBNode *NIL;  // Nó sentinela (folha preta)

void initializeNIL() {
    NIL = malloc(sizeof(RBNode));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
}

// Cria um novo nó vermelho com filhos NIL
RBNode* createNode(Vendas newProd) {
    RBNode *node = malloc(sizeof(RBNode));
    node->produto = newProd;
    node->color = RED;
    node->left = node->right = node->parent = NIL;
    return node;
}

void leftRotate(RBNode **root, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if(y->left != NIL) 
		y->left->parent = x;
    y->parent = x->parent;
    if(x->parent == NIL)
		*root = y;
    else if(x == x->parent->left)  
			x->parent->left = y;
    	 else
		 	x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rightRotate(RBNode **root, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != NIL) 
		x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NIL)
		*root = x;
    else if (y == y->parent->left)  
			y->parent->left = x;
    	 else
		 	y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void insertFixup(RBNode **root, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

void rbInsert(RBNode **root, Vendas prod) {
    RBNode *z = createNode(prod);
    RBNode *y = NIL, *x = *root;
    while(x != NIL) {
        y = x;
        if(strcmp(z->produto.linha.nome, x->produto.linha.nome) < 0)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == NIL)
        *root = z;
    else if (strcmp(z->produto.linha.nome, y->produto.linha.nome) < 0)
        y->left = z;
    else
        y->right = z;
    z->left = z->right = NIL;
    insertFixup(root, z);
}

RBNode* treeMinimum(RBNode *x) {
    while(x->left != NIL) 
		x = x->left;
    return x;
}

void deleteFixup(RBNode **root, RBNode *x) {
    while(x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

void rbTransplant(RBNode **root, RBNode *u, RBNode *v) {
    if (u->parent == NIL)
		*root = v;
    else if (u == u->parent->left)
			u->parent->left = v;
    	 else
		 	u->parent->right = v;
    v->parent = u->parent;
}

void rbDelete(RBNode **root, RBNode *z) {
    RBNode *y = z;
    Color yOriginalColor = y->color;
    RBNode *x;
    if (z->left == NIL) {
        x = z->right;
        rbTransplant(root, z, z->right);
    } else 
		if (z->right == NIL) {
        	x = z->left;
        	rbTransplant(root, z, z->left);
    	} else {
        	y = treeMinimum(z->right);
        	yOriginalColor = y->color;
        	x = y->right;
        	if (y->parent == z) {
           		x->parent = y;
        	} else {
           		rbTransplant(root, y, y->right);
           		y->right = z->right;
           		y->right->parent = y;
        	}
        	rbTransplant(root, z, y);
            y->left = z->left;
            if (y->left != NIL)
                y->left->parent = y;
            y->color = z->color;
    	}
    if( yOriginalColor == BLACK )
        deleteFixup(root, x);
    free(z);
}

void inorder(RBNode *root) {
    if (root != NIL) {
        inorder(root->left);
        printf("%s(%s) ", root->produto.linha.nome, root->color == RED ? "R" : "B");
        inorder(root->right);
    }
}

RBNode* searchRb(RBNode *root, Vendas *produtoProcurado) {
    RBNode *cursor = root;
    while(cursor != NIL){
        if(strcmp(cursor->produto.linha.nome, produtoProcurado->linha.nome) == 0) {
            return cursor;
        } else if(strcmp(cursor->produto.linha.nome, produtoProcurado->linha.nome) > 0) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }
    return NULL;
}

// ============================================
// Impressao da arvore com tabulacao (para visualizacao)
// ============================================
void printTree(RBNode *r, int level) {
    if(r == NIL)
        return;
    // Imprime subarvore direita primeiro para que a raiz fique no centro
    printTree(r->right, level+1);
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%s(%s)\n", r->produto.linha.nome, r->color == RED ? "R" : "B");
    // Imprime subarvore esquerda
    printTree(r->left, level+1);
}

int main() {
    initializeNIL();
    RBNode *root = NIL;
    Vendas produtos[256];
    
    srand(time(NULL));
    createAllProducts(produtos);
    int n = 10;
    for (int i = 0; i < n; i++)
        rbInsert(&root, produtos[i]);

    printf("Arvore apos insercoes (in-order):\n");
    inorder(root);
    printf("\n\n");
	printTree(root, 1);
    // Remove alguns nos
    const Vendas toDelete[] = {produtos[1], produtos[0], produtos[4] };
    int m = sizeof(toDelete)/sizeof(toDelete[0]);
    for (int i = 0; i < m; i++) {
        printf("Removendo %s...\n", toDelete[i]);
        // Buscar nó com chave toDelete[i]
        RBNode *z = root;
        while (z != NIL && strcmp(z->produto.linha.nome, toDelete[i].linha.nome) != 0) {
            z = (strcmp(toDelete[i].linha.nome, z->produto.linha.nome) < 0) ? z->left : z->right;
        }
        if (z != NIL)
            rbDelete(&root, z);
        inorder(root);
        printf("\n\n");
    }
	printTree(root, 1);
    RBNode* searched[3];

    Vendas toBeSearched[] = {produtos[0], produtos[2], produtos[3]};

    for(int i = 0; i < 3; i++) {
        searched[i] = searchRb(root, &toBeSearched[i]);
        if(searched[i] != NULL) {
            printf("Produto %s encontrado na árvore.\n", searched[i]->produto.linha.nome);
        } else {
            printf("Produto %s não encontrado na árvore.\n", toBeSearched[i].linha.nome);
        }
    }
    return 0;
}

