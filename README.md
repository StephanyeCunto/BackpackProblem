# Algoritmos para o Problema da Mochila

Este repositório contém duas implementações diferentes para resolver o Problema da Mochila - um problema clássico de otimização combinatória. São implementadas duas abordagens meta-heurísticas: Algoritmo Genético e Busca Tabu.

[![wakatime](https://wakatime.com/badge/user/5a343522-23db-45ae-b20b-54655c392390/project/0132b1dd-c354-439b-85b2-11fd930bb17e.svg)](https://wakatime.com/badge/user/5a343522-23db-45ae-b20b-54655c392390/project/0132b1dd-c354-439b-85b2-11fd930bb17e)

## Visão Geral do Problema

O Problema da Mochila consiste em selecionar itens com diferentes pesos e valores para maximizar o valor total, respeitando uma restrição de capacidade de peso. É um problema NP-difícil clássico com aplicações em diversas áreas.

## Implementações

### 1. Algoritmo Genético (`algoritmoGenetico.c`)
- Utiliza conceitos de evolução natural
- Implementa operadores genéticos:
  - Seleção por torneio
  - Crossover uniforme
  - Mutação
  - Elitismo
- População de soluções evoluindo ao longo de gerações

### 2. Busca Tabu (`algoritmoTabu.c`)
- Utiliza memória de curto prazo (lista tabu)
- Explora vizinhança de soluções
- Implementa:
  - Lista tabu circular
  - Geração de vizinhança
  - Critério de aspiração
  - Intensificação e diversificação

## Estrutura do Repositório

```
.
├── AlgortimoGenetico/
│   ├── algortimoGenetico.c
│   └── README.md
├── AlgortimoTabu/
|    ├── algortimoTabu.c
│   └── README.md
└── README.md
```

## Comparação das Implementações

| Característica          | Algoritmo Genético | Busca Tabu       |
|------------------------|-------------------|------------------|
| Parâmetros Principais  | População, Gerações, Taxas | Tamanho Lista, Vizinhança |
| Exploração do Espaço   | População diversa | Memória de movimentos |
| Memória                | Não utiliza       | Lista tabu      |
| Convergência           | Gradual           | Mais rápida     |
| Diversificação         | Natural           | Por vizinhança  |

## Compilação e Execução

### Pré-requisitos
- GCC

### Compilando
```bash
#Compilando Algoritmo Genetico
cd AlgoritmoGenetico

gcc algoritmoGenetico.c -o algoritmoGenetico.exe

#Compilando Algoritmo Tabu
cd AlgoritmoTabu

gcc algoritmoTabu.c -o algoritmoTabu.exe
```

### Executando
```bash
#Executando Algoritmo Genetico
./algoritmoGenetico.exe

#Executando Algoritmo Tabu
./algoritmoTabu.exe
```

## Parâmetros de Configuração

### Algoritmo Genético
```c
#define NUM_ITENS 26           // Número de itens
#define TAM_POPULACAO 100      // Tamanho da população
#define NUM_GERACOES 100       // Número de gerações
#define TAXA_MUTACAO 0.15      // Taxa de mutação
#define TAXA_ELITISMO 0.1      // Taxa de elitismo
#define CAPACIDADE 10          // Capacidade da mochila
```

### Busca Tabu
```c
#define NUM_ITENS 50           // Número de itens
#define TABU_SIZE 20          // Tamanho da lista tabu
#define MAX_ITERATIONS 1000    // Máximo de iterações
#define VIZINHOS_SIZE 20      // Tamanho da vizinhança
#define CAPACIDADE 20         // Capacidade da mochila
```

## Formato da Saída

Ambas as implementações fornecem:
1. Lista dos itens disponíveis
2. Progresso da execução
3. Solução final detalhada

### Exemplo de Saída (comum a ambos)
```
+-----------+--------+--------+
| Item      | Peso   | Valor  |
+-----------+--------+--------+
| Item 1    |      5 |     12 |
...
```

## Licença

Este projeto está sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.


## Referências

- "Genetic Algorithms in Search, Optimization, and Machine Learning" - D.E. Goldberg
- "Tabu Search" - Fred Glover
- "Meta-heuristics: Theory and Applications" - I.H. Osman, J.P. Kelly