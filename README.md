# Solucionador do Problema da Mochila
[![wakatime](https://wakatime.com/badge/user/5a343522-23db-45ae-b20b-54655c392390/project/0132b1dd-c354-439b-85b2-11fd930bb17e.svg)](https://wakatime.com/badge/user/5a343522-23db-45ae-b20b-54655c392390/project/0132b1dd-c354-439b-85b2-11fd930bb17e)

Este repositório fornece duas implementações meta-heurísticas para resolver o Problema da Mochila 0/1: um Algoritmo Genético e uma Busca Tabu. Ambas as abordagens oferecem diferentes estratégias para encontrar soluções ótimas ou próximas do ótimo para este problema de otimização combinatória NP-difícil.

## Descrição do Problema

O Problema da Mochila 0/1 envolve:
- Um conjunto de N itens, cada um com um peso e valor
- Uma mochila com capacidade máxima de peso
- Objetivo: Maximizar o valor total mantendo o peso total dentro da capacidade
- Cada item só pode ser selecionado uma vez (daí o "0/1")

## Características das Implementações

### Algoritmo Genético (`algoritmo_genetico.c`)
A implementação do algoritmo genético apresenta:

- **Estrutura do Cromossomo**
  - Representação binária (0/1 para cada item)
  - Mapeamento direto para seleção de itens
  - Tratamento automático da restrição de peso

- **Operadores Genéticos**
  - Seleção por Torneio (tamanho = 3)
  - Crossover Uniforme
  - Mutação por Troca de Bit
  - Elitismo (preserva melhores soluções)

- **Parâmetros**
  ```c
  #define NUM_ITENS 50          // Número de itens disponíveis
  #define TAM_POPULACAO 300     // Tamanho da população
  #define NUM_GERACOES 300      // Número de gerações
  #define TAXA_MUTACAO 0.1      // Taxa de mutação
  #define TAXA_ELITISMO 0.3     // Taxa de elitismo
  #define CAPACIDADE 20         // Capacidade da mochila
  ```

### Busca Tabu (`busca_tabu.c`)
A implementação da busca tabu inclui:

- **Representação da Solução**
  - Array binário para seleção de itens
  - Geração eficiente de vizinhança
  - Verificação integrada de viabilidade do peso

- **Componentes da Busca**
  - Lista tabu circular
  - Critérios de aspiração
  - Oscilação estratégica
  - Exploração de vizinhança

- **Parâmetros**
  ```c
  #define NUM_ITENS 50          // Número de itens
  #define TABU_SIZE 20          // Tamanho da lista tabu
  #define MAX_ITERATIONS 1000   // Máximo de iterações
  #define VIZINHOS_SIZE 200     // Tamanho da vizinhança
  #define CAPACIDADE 20         // Capacidade da mochila
  ```

## Compilação e Execução

### Pré-requisitos
- Compilador GCC
- Make (opcional)
- Ambiente Linux/Unix (para os comandos fornecidos)

### Compilação

```bash
# Compilar Algoritmo Genético
gcc -o algoritmo_genetico algoritmo_genetico.c -Wall -O2

# Compilar Busca Tabu
gcc -o busca_tabu busca_tabu.c -Wall -O2
```

### Execução

```bash
# Executar Algoritmo Genético
./algoritmo_genetico

# Executar Busca Tabu
./busca_tabu
```

## Formato da Saída

Ambas as implementações fornecem saída detalhada em três etapas:

1. **Configuração Inicial**
   ```
   +-----------+--------+--------+
   | Item      | Peso   | Valor  |
   +-----------+--------+--------+
   | Item 1    |      7 |     15 |
   | Item 2    |      3 |      8 |
   ...
   ```

2. **Acompanhamento do Progresso**
   - Algoritmo Genético: Mostra melhor fitness e fitness médio a cada 10 gerações
   - Busca Tabu: Exibe melhor solução encontrada a cada 10 iterações

3. **Solução Final**
   ```
   Melhor solução encontrada:
   Cromossomo: 1 0 1 0 1 ...
   Fitness: 145
   Peso Total: 19
   
   Itens selecionados:
   +-----------+--------+--------+
   | Item      | Peso   | Valor  |
   +-----------+--------+--------+
   | Item 1    |      7 |     15 |
   ...
   ```

## Comparação dos Algoritmos

| Aspecto                  | Algoritmo Genético            | Busca Tabu                     |
|-------------------------|------------------------------|--------------------------------|
| Estratégia de Busca     | Evolução populacional        | Trajetória de solução única    |
| Exploração do Espaço    | População diversa            | Memória de movimentos          |
| Uso de Memória          | Sem memória explícita        | Memória de curto prazo         |
| Sensibilidade Parâmetros| Mais parâmetros para ajustar | Menos parâmetros               |
| Custo Computacional     | Maior (baseado em população) | Menor (solução única)          |

## Estrutura do Projeto


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

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](License) para detalhes.

## Referências

1. Goldberg, D. E. (1989). Genetic Algorithms in Search, Optimization, and Machine Learning. Addison-Wesley.
2. Glover, F. (1989). Tabu Search—Part I. ORSA Journal on Computing, 1(3), 190-206.
3. Pisinger, D. (1995). Algorithms for Knapsack Problems. Tese de Ph.D., Universidade de Copenhagen.
4. Martello, S., & Toth, P. (1990). Knapsack Problems: Algorithms and Computer Implementations. Wiley.
