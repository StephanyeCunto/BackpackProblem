#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h>

// Constants
#define NUM_ITENS 5
#define TAM_POPULACAO 100  // Increased population size
#define NUM_GERACOES 100   // Increased generations
#define TAXA_MUTACAO 0.15  // Slightly increased mutation rate
#define TAXA_ELITISMO 0.1  // Elite preservation rate
#define CAPACIDADE 10
#define TAMANHO_TORNEIO 3  // Tournament size

// Structures
typedef struct {
    int peso;
    int valor;
    char nome[20];
    float razao_valor_peso;  // Added value/weight ratio
} Item;

typedef struct {
    int *cromossomo;
    int fitness;
    int peso_total;
    float razao_fitness;  // Normalized fitness
} Individuo;

typedef struct {
    Individuo *individuos;
    int tamanho;
    int melhor_fitness;
    float fitness_medio;
} Populacao;

// Global variables
Item itens[NUM_ITENS] = {
    {3, 3, "Item 1", 0},
    {3, 3, "Item 2", 0},
    {3, 1, "Item 3", 0},
    {3, 4, "Item 4", 0},
    {3, 2, "Item 5", 0}
};

// Function prototypes
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

// Initialize value/weight ratios
void inicializar_razoes_valor_peso(void) {
    for (int i = 0; i < NUM_ITENS; i++) {
        itens[i].razao_valor_peso = (float)itens[i].valor / itens[i].peso;
    }
}

// Create new individual
Individuo* criar_individuo(void) {
    Individuo *ind = (Individuo*)malloc(sizeof(Individuo));
    if (!ind) {
        fprintf(stderr, "Erro: Falha na alocação de memória para indivíduo\n");
        exit(1);
    }
    ind->cromossomo = (int*)malloc(NUM_ITENS * sizeof(int));
    if (!ind->cromossomo) {
        fprintf(stderr, "Erro: Falha na alocação de memória para cromossomo\n");
        free(ind);
        exit(1);
    }
    return ind;
}

// Destroy individual
void destruir_individuo(Individuo *ind) {
    if (ind) {
        free(ind->cromossomo);
        free(ind);
    }
}

// Create population
Populacao* criar_populacao(int tamanho) {
    Populacao *pop = (Populacao*)malloc(sizeof(Populacao));
    if (!pop) {
        fprintf(stderr, "Erro: Falha na alocação de memória para população\n");
        exit(1);
    }
    pop->individuos = (Individuo*)malloc(tamanho * sizeof(Individuo));
    if (!pop->individuos) {
        fprintf(stderr, "Erro: Falha na alocação de memória para indivíduos\n");
        free(pop);
        exit(1);
    }
    pop->tamanho = tamanho;
    pop->melhor_fitness = 0;
    pop->fitness_medio = 0;
    
    for (int i = 0; i < tamanho; i++) {
        pop->individuos[i].cromossomo = (int*)malloc(NUM_ITENS * sizeof(int));
        if (!pop->individuos[i].cromossomo) {
            fprintf(stderr, "Erro: Falha na alocação de memória para cromossomo\n");
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(pop->individuos[j].cromossomo);
            }
            free(pop->individuos);
            free(pop);
            exit(1);
        }
    }
    return pop;
}

// Destroy population
void destruir_populacao(Populacao *pop) {
    if (pop) {
        for (int i = 0; i < pop->tamanho; i++) {
            free(pop->individuos[i].cromossomo);
        }
        free(pop->individuos);
        free(pop);
    }
}

// Calculate fitness with weight penalty
int calcular_fitness(Individuo *ind) {
    int peso_total = 0, valor_total = 0;
    
    for (int i = 0; i < NUM_ITENS; i++) {
        if (ind->cromossomo[i]) {
            peso_total += itens[i].peso;
            valor_total += itens[i].valor;
        }
    }
    
    ind->peso_total = peso_total;
    
    // Apply penalty if weight exceeds capacity
    if (peso_total > CAPACIDADE) {
        return 0;
    }
    
    return valor_total;
}

// Initialize population
void inicializar_populacao(Populacao *pop) {
    for (int i = 0; i < pop->tamanho; i++) {
        for (int j = 0; j < NUM_ITENS; j++) {
            pop->individuos[i].cromossomo[j] = rand() % 2;
        }
        pop->individuos[i].fitness = calcular_fitness(&pop->individuos[i]);
    }
    atualizar_estatisticas_populacao(pop);
}

// Tournament selection
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

// Uniform crossover
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

// Mutation
void mutacao(Individuo *ind) {
    for (int i = 0; i < NUM_ITENS; i++) {
        if ((float)rand() / RAND_MAX < TAXA_MUTACAO) {
            ind->cromossomo[i] = !ind->cromossomo[i];
        }
    }
    ind->fitness = calcular_fitness(ind);
}

// Update population statistics
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

// Preserve elite individuals
void preservar_elitismo(Populacao *antiga, Populacao *nova) {
    int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);
    
    // Sort old population by fitness
    for (int i = 0; i < antiga->tamanho - 1; i++) {
        for (int j = 0; j < antiga->tamanho - i - 1; j++) {
            if (antiga->individuos[j].fitness < antiga->individuos[j + 1].fitness) {
                Individuo temp = antiga->individuos[j];
                antiga->individuos[j] = antiga->individuos[j + 1];
                antiga->individuos[j + 1] = temp;
            }
        }
    }
    
    // Copy elite individuals
    for (int i = 0; i < num_elite; i++) {
        memcpy(nova->individuos[i].cromossomo, antiga->individuos[i].cromossomo, NUM_ITENS * sizeof(int));
        nova->individuos[i].fitness = antiga->individuos[i].fitness;
    }
}

// Print statistics
void imprimir_estatisticas(int geracao, Populacao *pop) {
    printf("Geração %d: Melhor Fitness = %d, Fitness Médio = %.2f\n",
           geracao, pop->melhor_fitness, pop->fitness_medio);
}

// Print solution
void imprimir_solucao(Individuo *ind) {
    printf("\nMelhor solução encontrada:\n");
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

// Main genetic algorithm function
void algoritmo_genetico(void) {
    inicializar_razoes_valor_peso();
    
    Populacao *pop_atual = criar_populacao(TAM_POPULACAO);
    Populacao *pop_nova = criar_populacao(TAM_POPULACAO);
    
    inicializar_populacao(pop_atual);
    
    // Main loop
    for (int gen = 0; gen < NUM_GERACOES; gen++) {
        // Elitism
        preservar_elitismo(pop_atual, pop_nova);
        
        // Generate new individuals
        int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);
        for (int i = num_elite; i < TAM_POPULACAO - 1; i += 2) {
            Individuo *pai1 = selecao_torneio(pop_atual);
            Individuo *pai2 = selecao_torneio(pop_atual);
            
            crossover_uniforme(pai1, pai2, &pop_nova->individuos[i], 
                             &pop_nova->individuos[i + 1]);
            
            mutacao(&pop_nova->individuos[i]);
            mutacao(&pop_nova->individuos[i + 1]);
        }
        
        // Update statistics
        atualizar_estatisticas_populacao(pop_nova);
        
        // Print progress
        if (gen % 10 == 0) {
            imprimir_estatisticas(gen, pop_nova);
        }
        
        // Swap populations
        Populacao *temp = pop_atual;
        pop_atual = pop_nova;
        pop_nova = temp;
    }
    
    // Find best solution
    Individuo *melhor = &pop_atual->individuos[0];
    for (int i = 1; i < TAM_POPULACAO; i++) {
        if (pop_atual->individuos[i].fitness > melhor->fitness) {
            melhor = &pop_atual->individuos[i];
        }
    }
    
    imprimir_solucao(melhor);
    
    // Cleanup
    destruir_populacao(pop_atual);
    destruir_populacao(pop_nova);
}

int main(void) {
    srand((unsigned)time(NULL));
    algoritmo_genetico();
    return 0;
}