 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura de nó da pilha para double
typedef struct No {
    double valor;
    struct No *proximo;
} No;

// Estrutura da pilha
typedef struct {
    No *topo;
    int tamanho;
} Pilha;

// Cria uma nova pilha vazia
Pilha* criarPilha() {
    Pilha *pilha = (Pilha*) malloc(sizeof(Pilha));
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

// Insere um valor no topo da pilha
void empilhar(Pilha *pilha, double valor) {
    No *novoNo = (No*) malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->proximo = pilha->topo;
    pilha->topo = novoNo;
    pilha->tamanho++;
    printf("[DEBUG] Empilhou: %.6g\n", valor);
}

// Remove o topo da pilha e retorna seu valor
double desempilhar(Pilha *pilha) {
    if (pilha->topo == NULL) {
        printf("Erro: pilha vazia!\n");
        exit(EXIT_FAILURE);
    }

    No *remover = pilha->topo;
    double valor = remover->valor;
    pilha->topo = remover->proximo;
    free(remover);
    pilha->tamanho--;
    printf("[DEBUG] Desempilhou: %.6g\n", valor);
    return valor;
}

// Libera toda a memória da pilha
void liberarPilha(Pilha *pilha) {
    No *atual = pilha->topo;
    while (atual != NULL) {
        No *aux = atual;
        atual = atual->proximo;
        free(aux);
    }
    free(pilha);
}

// Verifica se a string representa um número inteiro ou real
int ehNumero(char *token) {
    if (*token == '\0') return 0; // vazio
    int i = 0;
    int ponto = 0;

    // Aceita sinal só no começo
    if (token[0] == '+' || token[0] == '-') i++;

    if (token[i] == '\0') return 0; // só sinal, não é número válido

    for (; token[i] != '\0'; i++) {
        if (token[i] == '.') {
            if (ponto) return 0; // mais de um ponto
            ponto = 1;
        } else if (!isdigit(token[i])) {
            return 0;
        }
    }
    return 1;
}

// Mostra o conteúdo da pilha (debug)
void mostrarPilha(Pilha *pilha) {
    printf("[DEBUG] Pilha atual: ");
    No *atual = pilha->topo;
    while (atual) {
        printf("%.6g ", atual->valor);
        atual = atual->proximo;
    }
    printf("\n");
}

// Avalia a expressão RPN lendo token por token
double avaliarRPN() {
    Pilha *pilha = criarPilha();
    char token[50];

    printf("Digite os elementos da expressão RPN (um por um, digite '=' para calcular):\n> ");

    while (scanf("%49s", token) == 1) {
        if (strcmp(token, "=") == 0) break;

        if (ehNumero(token)) {
            double valor = atof(token);
            empilhar(pilha, valor);
        } else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            if (pilha->tamanho < 2) {
                printf("Erro: operadores insuficientes na pilha!\n");
                liberarPilha(pilha);
                exit(EXIT_FAILURE);
            }

            double b = desempilhar(pilha);
            double a = desempilhar(pilha);
            double resultado;

            switch (token[0]) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/':
                    if (b == 0.0) {
                        printf("Erro: divisão por zero!\n");
                        liberarPilha(pilha);
                        exit(EXIT_FAILURE);
                    }
                    resultado = a / b;
                    break;
                default:
                    printf("Erro: operador desconhecido '%s'\n", token);
                    liberarPilha(pilha);
                    exit(EXIT_FAILURE);
            }

            printf("[DEBUG] Operação: %.6g %c %.6g = %.6g\n", a, token[0], b, resultado);
            empilhar(pilha, resultado);
        } else {
            printf("Erro: token inválido '%s'\n", token);
            liberarPilha(pilha);
            exit(EXIT_FAILURE);
        }

        mostrarPilha(pilha);
        printf("> ");
    }

    double resultadoFinal = desempilhar(pilha);

    if (pilha->tamanho != 0) {
        printf("Erro: expressão mal formada!\n");
        mostrarPilha(pilha);
        liberarPilha(pilha);
        exit(EXIT_FAILURE);
    }

    liberarPilha(pilha);
    return resultadoFinal;
}

int main() {
    double resultado = avaliarRPN();
    printf("Resultado: %.6g\n", resultado);
    return 0;
}
