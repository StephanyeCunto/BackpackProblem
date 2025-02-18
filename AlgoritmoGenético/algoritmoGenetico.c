#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <float.h>

#define NUM_ITENS 26
#define TAM_POPULACAO 100  
#define NUM_GERACOES 100 
#define TAXA_MUTACAO 0.15  
#define TAXA_ELITISMO 0.1  
#define CAPACIDADE 10
#define TAMANHO_TORNEIO 3  

typedef struct {
    int peso;
    int valor;
    char nome[20];
} Item;

typedef struct {
    int *cromossomo;
    int fitness;
    int peso_total;
    float razao_fitness;  
} Individuo;

typedef struct {
    Individuo *individuos;
    int tamanho;
    int melhor_fitness;
    float fitness_medio;
} Populacao;

Item itens[NUM_ITENS];

void inicializar_razoes_valor_peso(void);
Individuo* criar_individuo(void);
void destruir_individuo(Individuo *ind);
Populacao* criar_populacao(int tamanho);
void destruir_populacao(Populacao *pop);
int calcular_fitness(Individuo *ind);
void inicializar_populacao(Populacao *pop);
Individuo* selecao_torneio(Populacao *pop);
void crossover_uniforme(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2);
void mutacao(Individuo *ind);
void atualizar_estatisticas_populacao(Populacao *pop);
void preservar_elitismo(Populacao *antiga, Populacao *nova);
void imprimir_estatisticas(int geracao, Populacao *pop);
void imprimir_solucao(Individuo *ind);

void inicializar_itens(void) {
    for (int i = 0; i < NUM_ITENS; i++) {
        itens[i].peso = rand() % 10 + 1;
        itens[i].valor = rand() % 20 + 1;        
    }
}

void imprimir_itens(void) {
    for (int i = 0; i < NUM_ITENS; i++) {
        printf("Nome: %s, Peso: %d, Valor: %d, Razao (Valor/Peso): %.2f\n", 
                itens[i].nome, itens[i].peso, itens[i].valor);
    }
}

void destruir_individuo(Individuo *ind) {
    if (ind) {
        free(ind->cromossomo);
        free(ind);
    }
}

Populacao* criar_populacao(int tamanho) {
    Populacao *pop = (Populacao*)malloc(sizeof(Populacao));
   
    pop->individuos = (Individuo*)malloc(tamanho * sizeof(Individuo));
    
    pop->tamanho = tamanho;
    pop->melhor_fitness = 0;
    pop->fitness_medio = 0;
    
    for (int i = 0; i < tamanho; i++) {
        pop->individuos[i].cromossomo = (int*)malloc(NUM_ITENS * sizeof(int));
    }
    return pop;
}

void destruir_populacao(Populacao *pop) {
    if (pop) {
        for (int i = 0; i < pop->tamanho; i++) {
            free(pop->individuos[i].cromossomo);
        }
        free(pop->individuos);
        free(pop);
    }
}

int calcular_fitness(Individuo *ind) {
    int peso_total = 0, valor_total = 0;
    
    for (int i = 0; i < NUM_ITENS; i++) {
        if (ind->cromossomo[i]) {
            peso_total += itens[i].peso;
            valor_total += itens[i].valor;
        }
    }
    
    ind->peso_total = peso_total;
    
    if (peso_total > CAPACIDADE) {
        return 0;
    }
    
    return valor_total;
}

void inicializar_populacao(Populacao *pop) {
    for (int i = 0; i < pop->tamanho; i++) {
        for (int j = 0; j < NUM_ITENS; j++) {
            pop->individuos[i].cromossomo[j] = rand() % 2;
        }
        pop->individuos[i].fitness = calcular_fitness(&pop->individuos[i]);
    }
    atualizar_estatisticas_populacao(pop);
}

Individuo* selecao_torneio(Populacao *pop) {
    int melhor_idx = rand() % pop->tamanho;
    int melhor_fitness = pop->individuos[melhor_idx].fitness;
    
    for (int i = 1; i < TAMANHO_TORNEIO; i++) {
        int idx = rand() % pop->tamanho;
        if (pop->individuos[idx].fitness > melhor_fitness) {
            melhor_idx = idx;
            melhor_fitness = pop->individuos[idx].fitness;
        }
    }
    
    return &pop->individuos[melhor_idx];
}

void crossover_uniforme(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2) {
    for (int i = 0; i < NUM_ITENS; i++) {
        if (rand() % 2) {
            filho1->cromossomo[i] = pai1->cromossomo[i];
            filho2->cromossomo[i] = pai2->cromossomo[i];
        } else {
            filho1->cromossomo[i] = pai2->cromossomo[i];
            filho2->cromossomo[i] = pai1->cromossomo[i];
        }
    }
    
    filho1->fitness = calcular_fitness(filho1);
    filho2->fitness = calcular_fitness(filho2);
}

void mutacao(Individuo *ind) {
    for (int i = 0; i < NUM_ITENS; i++) {
        if ((float)rand() / RAND_MAX < TAXA_MUTACAO) {
            ind->cromossomo[i] = !ind->cromossomo[i];
        }
    }
    ind->fitness = calcular_fitness(ind);
}

void atualizar_estatisticas_populacao(Populacao *pop) {
    int soma_fitness = 0;
    pop->melhor_fitness = 0;
    
    for (int i = 0; i < pop->tamanho; i++) {
        soma_fitness += pop->individuos[i].fitness;
        if (pop->individuos[i].fitness > pop->melhor_fitness) {
            pop->melhor_fitness = pop->individuos[i].fitness;
        }
    }
    
    pop->fitness_medio = (float)soma_fitness / pop->tamanho;
}

void preservar_elitismo(Populacao *antiga, Populacao *nova) {
    int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);
    
    for (int i = 0; i < antiga->tamanho - 1; i++) {
        for (int j = 0; j < antiga->tamanho - i - 1; j++) {
            if (antiga->individuos[j].fitness < antiga->individuos[j + 1].fitness) {
                Individuo temp = antiga->individuos[j];
                antiga->individuos[j] = antiga->individuos[j + 1];
                antiga->individuos[j + 1] = temp;
            }
        }
    }
    
    for (int i = 0; i < num_elite; i++) {
        memcpy(nova->individuos[i].cromossomo, antiga->individuos[i].cromossomo, NUM_ITENS * sizeof(int));
        nova->individuos[i].fitness = antiga->individuos[i].fitness;
    }
}

void imprimir_estatisticas(int geracao, Populacao *pop) {
    printf("Geracao %d: Melhor Fitness = %d, Fitness Medio = %.2f\n",
           geracao, pop->melhor_fitness, pop->fitness_medio);
}

void imprimir_solucao(Individuo *ind) {
    printf("\nMelhor solucao encontrada:\n");
    printf("Cromossomo: ");
    for (int i = 0; i < NUM_ITENS; i++) {
        printf("%d ", ind->cromossomo[i]);
    }
    printf("\nFitness: %d\n", ind->fitness);
    printf("Peso Total: %d\n", ind->peso_total);
    printf("\nItens selecionados:\n");
    for (int i = 0; i < NUM_ITENS; i++) {
        if (ind->cromossomo[i]) {
            printf("- %s (Peso: %d, Valor: %d)\n", 
                   itens[i].nome, itens[i].peso, itens[i].valor);
        }
    }
}

void algoritmo_genetico(void) {
    
    Populacao *pop_atual = criar_populacao(TAM_POPULACAO);
    Populacao *pop_nova = criar_populacao(TAM_POPULACAO);
    
    inicializar_populacao(pop_atual);
    
    for (int gen = 0; gen < NUM_GERACOES; gen++) {
        preservar_elitismo(pop_atual, pop_nova);
        int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);
        for (int i = num_elite; i < TAM_POPULACAO - 1; i += 2) {
            Individuo *pai1 = selecao_torneio(pop_atual);
            Individuo *pai2 = selecao_torneio(pop_atual);
            
            crossover_uniforme(pai1, pai2, &pop_nova->individuos[i], 
                             &pop_nova->individuos[i + 1]);
            
            mutacao(&pop_nova->individuos[i]);
            mutacao(&pop_nova->individuos[i + 1]);
        }
        
        atualizar_estatisticas_populacao(pop_nova);
        
        if (gen % 10 == 0) {
            imprimir_estatisticas(gen, pop_nova);
        }
        
        Populacao *temp = pop_atual;
        pop_atual = pop_nova;
        pop_nova = temp;
    }
    
    Individuo *melhor = &pop_atual->individuos[0];
    for (int i = 1; i < TAM_POPULACAO; i++) {
        if (pop_atual->individuos[i].fitness > melhor->fitness) {
            melhor = &pop_atual->individuos[i];
        }
    }
    
    imprimir_solucao(melhor);
    
    destruir_populacao(pop_atual);
    destruir_populacao(pop_nova);
}

int main(void) {
    srand((unsigned)time(NULL));
    inicializar_itens(); 
    imprimir_itens();
    algoritmo_genetico();
    return 0;
}