#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ITENS 50
#define TABU_SIZE 20
#define MAX_ITERATIONS 1000
#define VIZINHOS_SIZE 100
#define CAPACIDADE 20

typedef struct {
    int peso;
    int valor;
    char nome[20];
} Item;

typedef struct {
    int *solucao;
    int fitness;
    int peso_total;
} Solucao;

typedef struct {
    int **solucoes;
    int tamanho;
    int inicio;
    int num_elementos;
} ListaTabu;

Item itens[NUM_ITENS];
ListaTabu *lista_tabu;

Solucao *criar_solucao(void);
void destruir_solucao(Solucao *s);
ListaTabu *criar_lista_tabu(int tamanho);
void destruir_lista_tabu(ListaTabu *lt);

void inicializar_itens(void);
void imprimir_itens(void);
int calcular_fitness(Solucao *s);
void gerar_solucao_inicial(Solucao *s);
void gerar_vizinho(Solucao *atual, Solucao *vizinho);
bool esta_na_lista_tabu(ListaTabu *lt, int *solucao);
void adicionar_lista_tabu(ListaTabu *lt, int *solucao);
void busca_tabu(void);
void imprimir_solucao(Solucao *s);
void imprimir_cabecario(void);
void imprimir_rodape(void);

Solucao *criar_solucao(void) {
    Solucao *s = (Solucao *)malloc(sizeof(Solucao));
    s->solucao = (int *)malloc(NUM_ITENS * sizeof(int));
    s->fitness = 0;
    s->peso_total = 0;
    return s;
}

void destruir_solucao(Solucao *s) {
    if (s) {
        free(s->solucao);
        free(s);
    }
}

ListaTabu *criar_lista_tabu(int tamanho) {
    ListaTabu *lt = (ListaTabu *)malloc(sizeof(ListaTabu));
    lt->solucoes = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++) {
        lt->solucoes[i] = (int *)malloc(NUM_ITENS * sizeof(int));
    }
    lt->tamanho = tamanho;
    lt->inicio = 0;
    lt->num_elementos = 0;
    return lt;
}

void destruir_lista_tabu(ListaTabu *lt) {
    if (lt) {
        for (int i = 0; i < lt->tamanho; i++) {
            free(lt->solucoes[i]);
        }
        free(lt->solucoes);
        free(lt);
    }
}

void inicializar_itens(void) {
    for (int i = 0; i < NUM_ITENS; i++) {
        sprintf(itens[i].nome, "Item %d", i + 1);
        itens[i].peso = rand() % 10 + 1;
        itens[i].valor = rand() % 20 + 1;
    }
}

void imprimir_itens(void) {
    printf("+-----------+--------+--------+\n");
    printf("| Item      | Peso   | Valor  |\n");
    printf("+-----------+--------+--------+\n");
    for (int i = 0; i < NUM_ITENS; i++) {
        printf("| %-9s | %6d | %6d |\n",
               itens[i].nome, itens[i].peso, itens[i].valor);
    }
    printf("+-----------+--------+--------+\n");
}

int calcular_fitness(Solucao *s) {
    int peso_total = 0, valor_total = 0;

    for (int i = 0; i < NUM_ITENS; i++) {
        if (s->solucao[i]) {
            peso_total += itens[i].peso;
            valor_total += itens[i].valor;
        }
    }

    if (peso_total > CAPACIDADE) {
        return 0;
    }

    s->peso_total = peso_total;

    return valor_total;
}

void gerar_solucao_inicial(Solucao *s) {
    int peso_atual = 0;
    for (int i = 0; i < NUM_ITENS; i++) {
        if (peso_atual + itens[i].peso < CAPACIDADE) {
            s->solucao[i] = rand() % 2;
            if (s->solucao[i]) {
                peso_atual += itens[i].peso;
            }
        } else {
            s->solucao[i] = 0;
        }
    }
    s->peso_total = peso_atual;
    s->fitness = calcular_fitness(s);
}

void gerar_vizinho(Solucao *atual, Solucao *vizinho) {
    memcpy(vizinho->solucao, atual->solucao, NUM_ITENS * sizeof(int));
    
    int pos1 = rand() % NUM_ITENS;
    int pos2;

    do {
        pos2 = rand() % NUM_ITENS;
    } while (pos1 == pos2);

    int novo_peso = atual->peso_total;
    if (vizinho->solucao[pos1]) {
        novo_peso -= itens[pos1].peso;
    } else {
        novo_peso += itens[pos1].peso;
    }
    if (vizinho->solucao[pos2]) {
        novo_peso -= itens[pos2].peso;
    } else {
        novo_peso += itens[pos2].peso;
    }

    if (novo_peso <= CAPACIDADE) {
        vizinho->solucao[pos1] = !vizinho->solucao[pos1];
        vizinho->solucao[pos2] = !vizinho->solucao[pos2];
        vizinho->peso_total = novo_peso;
        vizinho->fitness = calcular_fitness(vizinho);
    } else {
        vizinho->fitness = atual->fitness;
        vizinho->peso_total = atual->peso_total;
    }
}

bool esta_na_lista_tabu(ListaTabu *lt, int *solucao) {
    for (int i = 0; i < lt->num_elementos; i++) {
        int idx = (lt->inicio + i) % lt->tamanho;
        if (memcmp(lt->solucoes[idx], solucao, NUM_ITENS * sizeof(int)) == 0) {
            return true;
        }
    }
    return false;
}

void adicionar_lista_tabu(ListaTabu *lt, int *solucao) {
    memcpy(lt->solucoes[lt->inicio], solucao, NUM_ITENS * sizeof(int));
    lt->inicio = (lt->inicio + 1) % lt->tamanho;
    if (lt->num_elementos < lt->tamanho) {
        lt->num_elementos++;
    }
}

void busca_tabu(void) {
    Solucao *melhor_global = criar_solucao();
    Solucao *atual = criar_solucao();
    Solucao *melhor_vizinho = criar_solucao();
    
    lista_tabu = criar_lista_tabu(TABU_SIZE);

    gerar_solucao_inicial(atual);
    memcpy(melhor_global->solucao, atual->solucao, NUM_ITENS * sizeof(int));
    melhor_global->fitness = atual->fitness;
    melhor_global->peso_total = atual->peso_total;

    int iteracoes_sem_melhora = 0;
    
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        int melhor_fitness_vizinho = -1;
        
        for (int i = 0; i < VIZINHOS_SIZE; i++) {
            Solucao *vizinho = criar_solucao();
            gerar_vizinho(atual, vizinho);
            
            if (!esta_na_lista_tabu(lista_tabu, vizinho->solucao) && 
                vizinho->fitness > melhor_fitness_vizinho) {
                melhor_fitness_vizinho = vizinho->fitness;
                memcpy(melhor_vizinho->solucao, vizinho->solucao, NUM_ITENS * sizeof(int));
                melhor_vizinho->fitness = vizinho->fitness;
            }
            
            destruir_solucao(vizinho);
        }
        
        memcpy(atual->solucao, melhor_vizinho->solucao, NUM_ITENS * sizeof(int));
        atual->fitness = melhor_vizinho->fitness;
        
        adicionar_lista_tabu(lista_tabu, atual->solucao);
        
        if (atual->fitness > melhor_global->fitness) {
            memcpy(melhor_global->solucao, atual->solucao, NUM_ITENS * sizeof(int));
            melhor_global->fitness = atual->fitness;
            iteracoes_sem_melhora = 0;
        } else {
            iteracoes_sem_melhora++;
        }

        if (iter % 10 == 0) {
            printf("| %-8d | %14d |\n", iter, melhor_global->fitness);
        }
        
        if (iteracoes_sem_melhora > 100) {
            break;
        }
    }

    imprimir_rodape();
    imprimir_solucao(melhor_global);

    destruir_solucao(melhor_global);
    destruir_solucao(atual);
    destruir_solucao(melhor_vizinho);
    destruir_lista_tabu(lista_tabu);
}

void imprimir_solucao(Solucao *s) {
    printf("\nMelhor solucao encontrada:\n");
    printf("Solucao: ");
    for (int i = 0; i < NUM_ITENS; i++) {
        printf("%d ", s->solucao[i]);
    }
    printf("\nFitness: %d\n", s->fitness);
    printf("Peso Total: %d\n", s->peso_total);
    printf("\nItens selecionados:\n");
    printf("+-----------+--------+--------+\n");
    printf("| Item      | Peso   | Valor  |\n");
    printf("+-----------+--------+--------+\n");
    for (int i = 0; i < NUM_ITENS; i++) {
        if (s->solucao[i]) {
            printf("| %-9s | %6d | %6d |\n",
                   itens[i].nome, itens[i].peso, itens[i].valor);
        }
    }
    printf("+-----------+--------+--------+\n");
}

void imprimir_cabecario(void) {
    printf("+--------------------------------+\n");
    printf("| Iteracao | Melhor Fitness      |\n");
    printf("+--------------------------------+\n");
}

void imprimir_rodape(void) {
    printf("+--------------------------------+\n");
}

int main(void) {
    srand((unsigned)time(NULL));
    inicializar_itens();
    imprimir_itens();
    imprimir_cabecario();
    busca_tabu();
    
    return 0;
}