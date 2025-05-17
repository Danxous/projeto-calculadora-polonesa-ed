#include <stdio.h>
#include <stdlib.h>

// Estrutura de nó da pilha
typedef struct No {
    int valor;
    struct No *proximo;
} No;

// Estrutura da pilha propriamente dita
typedef struct {
    No *topo;      // Aponta para o topo da pilha
    int tamanho;   // Armazena o número de elementos na pilha
} Pilha;

// Função para criar uma nova pilha vazia
Pilha* criarPilha() {
    Pilha *pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

// Insere um novo valor no topo da pilha (push)
void empilhar(Pilha *pilha, int valor) {
    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->proximo = pilha->topo; // O novo nó aponta para o antigo topo
    pilha->topo = novoNo;          // O novo nó se torna o novo topo
    pilha->tamanho++;
}

// Remove e retorna o valor do topo da pilha (pop)
int desempilhar(Pilha *pilha) {
    if (pilha->topo == NULL) {
        printf("Pilha vazia! Não é possível desempilhar.\n");
        return -1; // Valor indicativo de erro
    }

    No *noRemovido = pilha->topo;
    int valor = noRemovido->valor;

    pilha->topo = noRemovido->proximo; // Avança o topo para o próximo
    free(noRemovido);
    pilha->tamanho--;

    return valor;
}

// Retorna o valor no topo da pilha (sem remover)
int topo(Pilha *pilha) {
    if (pilha->topo == NULL) {
        printf("Pilha vazia! Não há topo.\n");
        return -1;
    }
    return pilha->topo->valor;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->tamanho == 0;
}

// Imprime todos os elementos da pilha
void imprimirPilha(Pilha *pilha) {
    No *atual = pilha->topo;
    printf("Pilha (topo -> base): ");
    while (atual != NULL) {
        printf("%d ", atual->valor);
        atual = atual->proximo;
    }
    printf("\n");
}

// Imprime o tamanho da pilha
void imprimirTamanho(Pilha *pilha) {
    printf("Tamanho da pilha: %d\n", pilha->tamanho);
}

// Libera toda a memória alocada pela pilha
void liberarPilha(Pilha *pilha) {
    No *atual = pilha->topo;
    while (atual != NULL) {
        No *aux = atual;
        atual = atual->proximo;
        free(aux);
    }
    free(pilha);
}

// Função principal para testes
int main() {
    Pilha *pilha = criarPilha();

    empilhar(pilha, 10);
    empilhar(pilha, 20);
    empilhar(pilha, 30);
    printf("Após empilhar 10, 20 e 30:\n");
    imprimirPilha(pilha);
    imprimirTamanho(pilha);

    printf("Elemento no topo: %d\n", topo(pilha));

    int removido = desempilhar(pilha);
    printf("Valor desempilhado: %d\n", removido);
    imprimirPilha(pilha);
    imprimirTamanho(pilha);

    liberarPilha(pilha);
    return 0;
}
