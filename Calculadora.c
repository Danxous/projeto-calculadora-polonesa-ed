#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para alocação de memória e funções utilitárias
#include <string.h>  // Biblioteca para manipulação de strings
#include <ctype.h>   // Biblioteca para funções de manipulação de caracteres

// Estrutura de nó da pilha para armazenar valores do tipo double
typedef struct No {
    double valor;          // Valor armazenado no nó
    struct No *proximo;    // Ponteiro para o próximo nó na pilha
} No;

// Estrutura da pilha
typedef struct {
    No *topo;              // Ponteiro para o nó no topo da pilha
    int tamanho;           // Quantidade de elementos na pilha
} Pilha;

// Função para criar uma nova pilha vazia
Pilha* criarPilha() {
    Pilha *pilha = (Pilha*)malloc(sizeof(Pilha)); // Aloca memória para a pilha
    pilha->topo = NULL;                            // Inicializa o topo como NULL (pilha vazia)
    pilha->tamanho = 0;                            // Inicializa o tamanho como 0
    return pilha;                                  // Retorna a pilha criada
}

// Função para empilhar (inserir) um valor no topo da pilha
void empilhar(Pilha *pilha, double valor) {
    No *novoNo = (No*)malloc(sizeof(No));         // Aloca memória para um novo nó
    novoNo->valor = valor;                         // Define o valor do nó
    novoNo->proximo = pilha->topo;                 // Aponta para o nó que era o topo anterior
    pilha->topo = novoNo;                          // Atualiza o topo da pilha para o novo nó
    pilha->tamanho++;                              // Incrementa o tamanho da pilha
    printf("[DEBUG] Empilhou: %.6g\n", valor);     // Mensagem de depuração indicando o valor empilhado
}

// Função para desempilhar (remover) o valor do topo da pilha
double desempilhar(Pilha *pilha) {
    if (pilha->topo == NULL) {                     // Verifica se a pilha está vazia
        printf("Erro: pilha vazia!\n");           
        exit(EXIT_FAILURE);                        // Encerra o programa com erro
    }

    No *remover = pilha->topo;                     // Armazena o nó do topo para remoção
    double valor = remover->valor;                 // Armazena o valor do nó do topo
    pilha->topo = remover->proximo;                // Atualiza o topo para o próximo nó
    free(remover);                                 // Libera a memória do nó removido
    pilha->tamanho--;                              // Decrementa o tamanho da pilha
    printf("[DEBUG] Desempilhou: %.6g\n", valor); // Mensagem de depuração indicando o valor desempilhado
    return valor;                                  // Retorna o valor desempilhado
}

// Libera toda a memória da pilha
void liberarPilha(Pilha *pilha) {
    No *atual = pilha->topo; // Começa pelo topo da pilha
    while (atual != NULL) {  // Percorre todos os nós da pilha
        No *aux = atual;     // Armazena o nó atual para remoção
        atual = atual->proximo; // Avança para o próximo nó
        free(aux);           // Libera a memória do nó atual
    }
    free(pilha);             // Libera a memória da estrutura da pilha
}

// Verifica se a string representa um número inteiro ou real
int ehNumero(char *token) {
    if (*token == '\0') return 0; // Retorna 0 se a string estiver vazia
    int i = 0; // Índice para percorrer a string
    int ponto = 0; // Variável para verificar a presença de um ponto decimal

    // Aceita sinal (+ ou -) apenas no início
    if (token[0] == '+' || token[0] == '-') i++;

    if (token[i] == '\0') return 0; // Retorna 0 se houver apenas o sinal

    // Percorre o restante da string
    for (; token[i] != '\0'; i++) {
        if (token[i] == '.') { // Verifica se é um ponto decimal
            if (ponto) return 0; // Retorna 0 se houver mais de um ponto
            ponto = 1; // Marca que um ponto foi encontrado
        } else if (!isdigit(token[i])) { // Verifica se o caractere não é um dígito
            return 0; // Retorna 0 se não for um número válido
        }
    }
    return 1; // Retorna 1 se a string for um número válido
}

// Mostra o conteúdo da pilha (debug)
void mostrarPilha(Pilha *pilha) {
    printf("[DEBUG] Pilha atual: "); 
    No *atual = pilha->topo; // Começa pelo topo da pilha
    while (atual) { // Percorre todos os nós da pilha
        printf("%.6g ", atual->valor); // Exibe o valor armazenado no nó
        atual = atual->proximo; // Avança para o próximo nó
    }
    printf("\n"); // Quebra de linha após exibir todos os valores
}

// Avalia a expressão RPN lendo token por token
double avaliarRPN() {
    Pilha *pilha = criarPilha(); // Cria uma pilha para armazenar os valores
    char token[50]; // Buffer para armazenar o token lido

    printf("Digite os elementos da expressão RPN (um por um, digite '=' para calcular):\n> ");

    while (scanf("%49s", token) == 1) { // Lê os tokens da entrada padrão
        if (strcmp(token, "=") == 0) break; // Interrompe o loop ao encontrar '='

        if (ehNumero(token)) { // Verifica se o token é um número
            double valor = atof(token); // Converte o token para um número do tipo double
            empilhar(pilha, valor); // Empilha o número na pilha
        } else if (strlen(token) == 1 && strchr("+-*/", token[0])) { // Verifica se o token é um operador válido
            if (pilha->tamanho < 2) { // Verifica se há operandos suficientes na pilha
                printf("Erro: operadores insuficientes na pilha!\n"); 
                liberarPilha(pilha); // Libera a memória da pilha
                exit(EXIT_FAILURE); // Encerra o programa com erro
            }

            double b = desempilhar(pilha); // Desempilha o primeiro operando (b)
            double a = desempilhar(pilha); // Desempilha o segundo operando (a)
            double resultado; // Variável para armazenar o resultado da operação

            // Verifica o operador e realiza a operação correspondente
            switch (token[0]) {
                case '+': 
                    resultado = a + b; // Soma
                    break;
                case '-': 
                    resultado = a - b; // Subtração
                    break;
                case '*': 
                    resultado = a * b; // Multiplicação
                    break;
                case '/': 
                    if (b == 0.0) { // Verifica divisão por zero
                        printf("Erro: divisão por zero!\n"); 
                        liberarPilha(pilha); // Libera a memória da pilha
                        exit(EXIT_FAILURE); // Encerra o programa com erro
                    }
                    resultado = a / b; // Divisão
                    break;
                default: 
                    // Caso o operador seja desconhecido
                    printf("Erro: operador desconhecido '%s'\n", token); 
                    liberarPilha(pilha); // Libera a memória da pilha
                    exit(EXIT_FAILURE); // Encerra o programa com erro
            }

            // Exibe a operação realizada para depuração
            printf("[DEBUG] Operação: %.6g %c %.6g = %.6g\n", a, token[0], b, resultado);
            empilhar(pilha, resultado); // Empilha o resultado da operação
        } else {
            // Caso o token seja inválido (não número nem operador)
            printf("Erro: token inválido '%s'\n", token); 
            liberarPilha(pilha); // Libera a memória da pilha
            exit(EXIT_FAILURE); // Encerra o programa com erro
        }

        mostrarPilha(pilha); // Mostra o estado atual da pilha (para depuração)
        printf("> "); // Exibe o prompt para o próximo token
    }

    double resultadoFinal = desempilhar(pilha); // Obtém o resultado final da pilha

    if (pilha->tamanho != 0) { // Verifica se a pilha está vazia 
        printf("Erro: expressão mal formada!\n"); 
        mostrarPilha(pilha); // Mostra o estado da pilha para ajudar na depuração
        liberarPilha(pilha); // Libera a memória alocada para a pilha
        exit(EXIT_FAILURE); // Encerra o programa com erro
    }

    liberarPilha(pilha); // Libera a memória alocada para a pilha
    return resultadoFinal; // Retorna o resultado final da avaliação
}

// Retorna a prioridade do operador
int prioridade(char operador) {
    switch (operador) {
        case '+':
        case '-':
            return 1; // Menor prioridade
        case '*':
        case '/':
            return 2; // Maior prioridade
        default:
            return 0; // Não é operador
    }
}

// Identifica se um caractere é um operador matemático, retornando 1 ou 0
int ehOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Transforma expressões infixas para pós-fixadas
void converterRPN(const char *infixa, char *posfixa) {
    char pilhaOp[256]; // Pilha para operadores (array de char)
    int topo = -1; // Inicializa o topo da pilha de operadores
    int i = 0; // Índice para percorrer a expressão infixa
    int j = 0; // Índice para construir a expressão pós-fixada

    while (infixa[i] != '\0') { // Percorre toda a expressão infixa
        if (isspace(infixa[i])) { // Ignora espaços em branco
            i++;
            continue;
        }

        // Verifica se o caractere é um número ou um sinal seguido de número
        if (isdigit(infixa[i]) || ((infixa[i] == '+' || infixa[i] == '-') && isdigit(infixa[i + 1])) || (infixa[i] == '.' && isdigit(infixa[i + 1]))) {
            int k = 0; // Índice para construir o token numérico
            char token[64]; // Buffer para armazenar o token numérico
            if (infixa[i] == '+' || infixa[i] == '-') {
                token[k++] = infixa[i++]; // Adiciona o sinal ao token
            }
            while (isdigit(infixa[i]) || infixa[i] == '.') { // Lê o número
                token[k++] = infixa[i++];
            }
            token[k] = '\0'; // Termina a string do token

            // Adiciona o token à expressão pós-fixada
            int l = 0;
            while (token[l]) {
                posfixa[j++] = token[l++];
            }
            posfixa[j++] = ' '; // Adiciona espaço após o número
            continue;
        }

        // Se encontrar um parêntese de abertura, empilha
        if (infixa[i] == '(') {
            pilhaOp[++topo] = infixa[i++];
            continue;
        }

        // Se encontrar um parêntese de fechamento, desempilha até encontrar o parêntese de abertura
        if (infixa[i] == ')') {
            while (topo >= 0 && pilhaOp[topo] != '(') {
                posfixa[j++] = pilhaOp[topo--];
                posfixa[j++] = ' ';
            }
            if (topo >= 0 && pilhaOp[topo] == '(') topo--; // Remove o parêntese de abertura
            i++;
            continue;
        }

        // Se encontrar um operador, desempilha operadores de maior ou igual prioridade
        if (ehOperador(infixa[i])) {
            char operador = infixa[i];
            while (topo >= 0 && ehOperador(pilhaOp[topo]) && prioridade(pilhaOp[topo]) >= prioridade(operador)) {
                posfixa[j++] = pilhaOp[topo--];
                posfixa[j++] = ' ';
            }
            pilhaOp[++topo] = operador; // Empilha o operador atual
            i++;
            continue;
        }

        // Caso encontre um caractere inválido
        printf("Erro: caractere inválido '%c'\n", infixa[i]);
        exit(EXIT_FAILURE);
    }

    // Desempilha todos os operadores restantes
    while (topo >= 0) {
        if (pilhaOp[topo] == '(' || pilhaOp[topo] == ')') {
            printf("Erro: parênteses desbalanceados\n");
            exit(EXIT_FAILURE);
        }
        posfixa[j++] = pilhaOp[topo--];
        posfixa[j++] = ' ';
    }

    // Termina a string da expressão pós-fixada
    if (j > 0) {
        posfixa[j - 1] = '\0'; // Remove o último espaço
    } else {
        posfixa[0] = '\0'; // Se não houver tokens, define como string vazia
    }
}

int main() {
    int modo; // Variável para armazenar o modo selecionado

    // Menu de seleção de modo da calculadora
    printf("\n    Escolha o modo da calculadora:   ");
    printf("\n\n 1 - Calculadora Polonesa Reversa");
    printf("\n 2 - Conversão para Polonesa Reversa");
    printf("\n > ");

    // Validação da entrada: só aceita 1 ou 2
    if (scanf("%d%*c", &modo) != 1 || (modo != 1 && modo != 2)) {
        printf("Erro: entrada inválida!\n");
        return EXIT_FAILURE; // Encerra o programa com erro
    }

    if (modo == 1) {
        double resultado = avaliarRPN(); // Avalia a expressão RPN
        printf("Resultado: %.6g\n", resultado); // Exibe o resultado
    } 
    else if (modo == 2) {
        char infixa[256]; // Buffer para a expressão infixa
        char posfixa[256]; // Buffer para a expressão pós-fixada

        printf("Digite a expressão algébrica:\n> ");
        scanf("%255[^\n]%*c", infixa);  // Lê linha inteira até \n

        converterRPN(infixa, posfixa); // Converte a expressão infixa para pós-fixada
        printf("Expressão na forma Polonesa Reversa: %s\n", posfixa); // Exibe a expressão pós-fixada
    }

    return 0;
}
