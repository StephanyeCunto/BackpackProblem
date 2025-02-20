#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <float.h>

#define NUM_ITENS 50
#define TAM_POPULACAO 300
#define NUM_GERACOES 300
#define TAXA_MUTACAO 0.1
#define TAXA_ELITISMO 0.3
#define CAPACIDADE 20
#define TAMANHO_TORNEIO 3

typedef struct
{
    int peso;
    int valor;
    char nome[20];
} Item;

typedef struct
{
    int *cromossomo;
    int fitness;
    int peso_total;
} Individuo;

typedef struct
{
    Individuo *individuos;
    int tamanho;
    int melhor_fitness;
    float fitness_medio;
} Populacao;

Item itens[NUM_ITENS];

Populacao *criar_populacao(int tamanho);
void destruir_populacao(Populacao *pop);
int calcular_fitness(Individuo *ind);
void inicializar_populacao(Populacao *pop);
Individuo *selecao_torneio(Populacao *pop);
void crossover_uniforme(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2);
void mutacao(Individuo *ind);
void atualizar_estatisticas_populacao(Populacao *pop);
void preservar_elitismo(Populacao *antiga, Populacao *nova);
void imprimir_estatisticas(int geracao, Populacao *pop);
void imprimir_solucao(Individuo *ind);
void algoritmo_genetico(void);
void imprimir_cabecario(void);
void imprimir_rodape(void);
void inicializar_itens(void)
{
    for (int i = 0; i < NUM_ITENS; i++)
    {
        sprintf(itens[i].nome, "Item %d", i + 1);
        itens[i].peso = rand() % 10 + 1;
        itens[i].valor = rand() % 20 + 1;
    }
}

void imprimir_itens(void)
{
    printf("+-----------+--------+--------+\n");
    printf("| Item      | Peso   | Valor  |\n");
    printf("+-----------+--------+--------+\n");
    for (int i = 0; i < NUM_ITENS; i++)
    {
        printf("| %-9s | %6d | %6d |\n",
               itens[i].nome, itens[i].peso, itens[i].valor);
    }
    printf("+-----------+--------+--------+\n");
}

Populacao *criar_populacao(int tamanho)
{
    Populacao *pop = (Populacao *)malloc(sizeof(Populacao));

    pop->individuos = (Individuo *)malloc(tamanho * sizeof(Individuo));

    pop->tamanho = tamanho;
    pop->melhor_fitness = 0;
    pop->fitness_medio = 0;

    for (int i = 0; i < tamanho; i++)
    {
        pop->individuos[i].cromossomo = (int *)malloc(NUM_ITENS * sizeof(int));
    }
    return pop;
}

void destruir_populacao(Populacao *pop)
{
    if (pop)
    {
        for (int i = 0; i < pop->tamanho; i++)
        {
            free(pop->individuos[i].cromossomo);
        }
        free(pop->individuos);
        free(pop);
    }
}

int calcular_fitness(Individuo *ind)
{
    int peso_total = 0, valor_total = 0;

    for (int i = 0; i < NUM_ITENS; i++)
    {
        if (ind->cromossomo[i])
        {
            peso_total += itens[i].peso;
            valor_total += itens[i].valor;
        }
    }

    ind->peso_total = peso_total;

    if (peso_total > CAPACIDADE)
    {
        return 0;
    }

    return valor_total;
}

void criar_individuo(Individuo *individuo)
{
    int peso_atual = 0;
    for (int j = 0; j < NUM_ITENS; j++)
    {
        if (peso_atual + itens[j].peso < CAPACIDADE)
        {
            individuo->cromossomo[j] = rand() % 2;
            if (individuo->cromossomo[j])
            {
                peso_atual += itens[j].peso;
            }
        }
        else
        {
            individuo->cromossomo[j] = 0;
        }
    }
    individuo->fitness = calcular_fitness(individuo);
}

void inicializar_populacao(Populacao *pop)
{
    for (int i = 0; i < pop->tamanho; i++)
    {
        criar_individuo(&pop->individuos[i]);
    }
    atualizar_estatisticas_populacao(pop);
}

Individuo *selecao_torneio(Populacao *pop)
{
    int melhor_idx = rand() % pop->tamanho;
    int melhor_fitness = pop->individuos[melhor_idx].fitness;

    for (int i = 1; i < TAMANHO_TORNEIO; i++)
    {
        int idx = rand() % pop->tamanho;
        if (pop->individuos[idx].fitness > melhor_fitness && pop->individuos[idx].peso_total <= CAPACIDADE)
        {
            melhor_idx = idx;
            melhor_fitness = pop->individuos[idx].fitness;
        }
    }

    return &pop->individuos[melhor_idx];
}

void crossover_uniforme(Individuo *pai1, Individuo *pai2, Individuo *filho1, Individuo *filho2)
{
    for (int i = 0; i < NUM_ITENS; i++)
    {
        if (rand() % 2)
        {
            filho1->cromossomo[i] = pai1->cromossomo[i];
            filho2->cromossomo[i] = pai2->cromossomo[i];
        }
        else
        {
            filho1->cromossomo[i] = pai2->cromossomo[i];
            filho2->cromossomo[i] = pai1->cromossomo[i];
        }
    }

    filho1->fitness = calcular_fitness(filho1);
    filho2->fitness = calcular_fitness(filho2);
}

void mutacao(Individuo *ind)
{
    for (int i = 0; i < NUM_ITENS; i++)
    {
        if ((float)rand() / RAND_MAX < TAXA_MUTACAO)
        {
            ind->cromossomo[i] = !ind->cromossomo[i];
        }
    }
    ind->fitness = calcular_fitness(ind);
}

void atualizar_estatisticas_populacao(Populacao *pop)
{
    float soma_fitness = 0;
    pop->melhor_fitness = 0;

    for (int i = 0; i < pop->tamanho; i++)
    {
        soma_fitness += pop->individuos[i].fitness;
        if (pop->individuos[i].fitness > pop->melhor_fitness)
        {
            pop->melhor_fitness = pop->individuos[i].fitness;
        }
    }
    pop->fitness_medio = (float)soma_fitness / pop->tamanho;
}

void preservar_elitismo(Populacao *antiga, Populacao *nova)
{
    int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);

    for (int i = 0; i < antiga->tamanho - 1; i++)
    {
        for (int j = 0; j < antiga->tamanho - i - 1; j++)
        {
            if (antiga->individuos[j].fitness < antiga->individuos[j + 1].fitness)
            {
                Individuo temp = antiga->individuos[j];
                antiga->individuos[j] = antiga->individuos[j + 1];
                antiga->individuos[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < num_elite; i++)
    {
        memcpy(nova->individuos[i].cromossomo, antiga->individuos[i].cromossomo, NUM_ITENS * sizeof(int));
        nova->individuos[i].fitness = antiga->individuos[i].fitness;
    }
}

void imprimir_estatisticas(int geracao, Populacao *pop)
{
    printf("Geracao %d: Melhor Fitness = %d, Fitness Medio = %.2f\n",
           geracao, pop->melhor_fitness, pop->fitness_medio);
}

void imprimir_solucao(Individuo *ind)
{
    printf("\nMelhor solucao encontrada:\n");
    printf("Cromossomo: ");
    for (int i = 0; i < NUM_ITENS; i++)
    {
        printf("%d ", ind->cromossomo[i]);
    }
    printf("\nFitness: %d\n", ind->fitness);
    printf("Peso Total: %d\n", ind->peso_total);
    printf("\nItens selecionados:\n");
    printf("+-----------+--------+--------+\n");
    printf("| Item      | Peso   | Valor  |\n");
    printf("+-----------+--------+--------+\n");
    for (int i = 0; i < NUM_ITENS; i++)
    {
        if (ind->cromossomo[i])
        {
            printf("| %-9s | %6d | %6d |\n",
                   itens[i].nome, itens[i].peso, itens[i].valor);
        }
    }
    printf("+-----------+--------+--------+\n");
}

void algoritmo_genetico(void)
{

    Populacao *pop_atual = criar_populacao(TAM_POPULACAO);
    Populacao *pop_nova = criar_populacao(TAM_POPULACAO);

    inicializar_populacao(pop_atual);

    for (int gen = 0; gen < NUM_GERACOES; gen++)
    {
        preservar_elitismo(pop_atual, pop_nova);
        int num_elite = (int)(TAM_POPULACAO * TAXA_ELITISMO);
        for (int i = num_elite; i < TAM_POPULACAO - 1; i += 2)
        {
            Individuo *pai1 = selecao_torneio(pop_atual);
            Individuo *pai2 = selecao_torneio(pop_atual);

            crossover_uniforme(pai1, pai2, &pop_nova->individuos[i],
                               &pop_nova->individuos[i + 1]);

            mutacao(&pop_nova->individuos[i]);
            mutacao(&pop_nova->individuos[i + 1]);
        }

        atualizar_estatisticas_populacao(pop_nova);

        if (gen % 10 == 0 && gen != 0)
        {

            printf("| %-8d | %14d | %14.2f |\n",
                   gen, pop_nova->melhor_fitness, pop_nova->fitness_medio);
        }

        Populacao *temp = pop_atual;
        pop_atual = pop_nova;
        pop_nova = temp;
    }

    imprimir_rodape();

    Individuo *melhor = &pop_atual->individuos[0];
    for (int i = 1; i < TAM_POPULACAO; i++)
    {
        if (pop_atual->individuos[i].fitness > melhor->fitness)
        {
            melhor = &pop_atual->individuos[i];
        }
    }

    imprimir_solucao(melhor);

    destruir_populacao(pop_atual);
    destruir_populacao(pop_nova);
}

void imprimir_cabecario(void)
{
    printf("+---------------------------------------------+\n");
    printf("| Geracao  | Melhor Fitness | Fitness Medio  |\n");
    printf("+---------------------------------------------+\n");
}

void imprimir_rodape(void)
{
    printf("+---------------------------------------------+\n");
}

int main(void)
{
    srand((unsigned)time(NULL));
    inicializar_itens();
    imprimir_itens();
    imprimir_cabecario();
    algoritmo_genetico();

    return 0;
}