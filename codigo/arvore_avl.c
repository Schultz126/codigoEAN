#include <stdio.h>
#include <stdlib.h>

// Definicao do no da arvore AVL
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

// Funcao que retorna a altura do no
int height(AVLNode *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Funcao que retorna o maior entre dois inteiros
int max(int a, int b) {
    return (a > b)? a : b;
}

// Cria um novo no com a chave informada
AVLNode* newNode(int key) {
    AVLNode* node = (AVLNode*) malloc(sizeof(AVLNode));
    if(node == NULL) {
        printf("Erro de alocacao de memoria.\n");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // novo no eh inicialmente folha
    return node;
}

// Rotacao para a direita
AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    
    // Executa a rotacao
    x->right = y;
    y->left = T2;
    
    // Atualiza as alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    
    // Retorna a nova raiz
    return x;
}

// Rotacao para a esquerda
AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    
    // Executa a rotacao
    y->left = x;
    x->right = T2;
    
    // Atualiza as alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    
    // Retorna a nova raiz
    return y;
}

// Retorna o fator de balanceamento de um no
int getBalance(AVLNode *node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Funcao para inserir uma chave na arvore AVL
AVLNode* insert(AVLNode* node, int key) {
    // Insercao normal na arvore binaria de busca
    if (node == NULL)
        return newNode(key);
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // chaves duplicadas nao sao permitidas
        return node;
    
    // Atualiza a altura deste no
    node->height = 1 + max(height(node->left), height(node->right));
    
    // Calcula o fator de balanceamento deste no
    int balance = getBalance(node);
    
    // Caso Left Left
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    
    // Caso Right Right
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    
    // Caso Left Right
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    
    // Caso Right Left
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    // Retorna o no (inalterado)
    return node;
}

// Percurso in-order da arvore
void inOrder(AVLNode *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

// Percurso pre-order da arvore
void preOrder(AVLNode *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// Libera a memoria da arvore
void freeTree(AVLNode *root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}
// ============================================
// Impressao da arvore com tabulacao (para visualizacao)
// ============================================
void printTree(AVLNode *root, int level) {
    if(root == NULL)
        return;
    // Imprime subarvore direita primeiro para que a raiz fique no centro
    printTree(root->right, level + 1);
    for (int i = 0; i < level; i++) {
        printf("\t");
    }
    printf("%d\n", root->key);
    // Imprime subarvore esquerda
    printTree(root->left, level + 1);
}

int main() {
    AVLNode *root = NULL;
    int choice, key;
    
    while(1) {
        printf("\nMenu de Operacoes:\n");
        printf("1. Inserir chave\n");
        printf("2. Exibir in-order\n");
        printf("3. Exibir pre-order\n");
        printf("4. Sair\n");
        printf("5. Imprimir\n");
        do {
	        printf("Escolha uma opcao: ");
    	    scanf("%d", &choice);
		} while( choice < 1 || choice > 5);
        
        switch(choice) {
            case 1:
                printf("Digite uma chave para inserir: ");
                scanf("%d", &key);
                root = insert(root, key);
                break;
            case 2:
                printf("Exibindo in-order: ");
                inOrder(root);
                printf("\n");
                break;
            case 3:
                printf("Exibindo pre-order: ");
                preOrder(root);
                printf("\n");
                break;
            case 4:
                freeTree(root);
                exit(0);
            case 5:
            	printTree(root,0);
            	break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
    
    return 0;
}
