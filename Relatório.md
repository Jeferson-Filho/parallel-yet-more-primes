# Relatório - Yet More Primes (OpenMP)

Participantes: 

  - Jeferson Dietrich Filho
  - Murilo Fortunano
  - Caio Bohlhalter de Souza

## Apresentação do Problema

Este projeto aborda a resolução do problema **Yet More Primes**, proposto na Maratona de Programação Paralela da WSCAD.

Nesse problema, um conjunto de números primos é dividido em duas partes, que são embaralhadas. O objetivo é combinar todas as possíveis metades iniciais e finais, formando novos números, e verificar quais dessas concatenações também são números primos.

Neste projeto, a versão sequencial foi utilizada como referência, e desenvolvemos uma versão paralela utilizando **OpenMP**, permitindo distribuir o processamento entre múltiplas threads e reduzir o tempo de execução.

## Estratégia de Paralelização

A estratégia de paralelização adotada consiste em distribuir entre múltiplas threads o processamento das combinações entre as metades dos números.

Como cada teste de concatenação e verificação de primalidade é independente dos demais, o laço duplo responsável por gerar todas as combinações foi paralelizado utilizando OpenMP com a diretiva:

```c
#pragma omp for collapse(2)
```

Essa abordagem permite que as iterações dos dois laços sejam divididas de forma equilibrada entre as threads.

Além disso, foi utilizado o escalonamento dinâmico (`schedule(dynamic)`), que melhora o balanceamento de carga, já que o tempo necessário para verificar a primalidade pode variar conforme o número analisado.

Para evitar condições de corrida e reduzir o custo de sincronização, cada thread armazena localmente os números primos encontrados e, ao final da região paralela, esses resultados são reunidos em um único vetor compartilhado. Essa abordagem minimiza o uso de mecanismos de sincronização durante a execução, aumentando a eficiência da versão paralela.

## Metodologia de Testes

Os dados apresentados são resultado da média de **30 execuções** para cada um dos tamanhos de entrada testados:

10, 20, 50, 75, 100, 125

## Resultados

A seguir estão representados os resultados obtidos em cada teste e logo abaixo os gráficos sobre cada um dos dados apresntados na tabela

| Tamanho do Input (N) | Tempo Paralelo (ms) | Tempo Sequencial (ms) | Speedup | Eficiência (%) |
|----------------------:|--------------------:|----------------------:|---------:|---------------:|
| 10  | 206.57 | 213.40 | 1.03x | 6.5% |
| 20  | 309.61 | 438.85 | 1.42x | 8.9% |
| 50  | 945.54 | 4487.61 | 4.75x | 29.7% |
| 75  | 1686.03 | 8588.97 | 5.09x | 31.8% |
| 100 | 2412.38 | 13301.26 | 5.51x | 34.5% |
| 125 | 3517.41 | 22166.27 | 6.30x | 39.4% |

## Descrição dos Gráficos

### 1. Tempo de Execução (Linear)

Representa o tempo absoluto gasto, em milissegundos, pelas versões sequencial e paralela em função do tamanho da entrada.

<img width="940" height="700" alt="Tempo de Execução Linear" src="https://github.com/user-attachments/assets/e3b19db9-293d-4420-b4a9-11082d996832" />

---

### 2. Tempo de Execução (Logarítmica)

Representa os mesmos tempos de execução, porém com o eixo **Y em escala logarítmica**, facilitando a visualização da diferença entre os resultados para diferentes tamanhos de entrada.

<img width="940" height="702" alt="Tempo de Execução Logarítmica" src="https://github.com/user-attachments/assets/99023733-b5e3-4b3f-9eca-75778937ce24" />

---

### 3. Escalabilidade (Speedup)

Representa a razão entre o tempo de execução sequencial e o tempo de execução paralelo, indicando quantas vezes a versão paralela é mais rápida.

<img width="940" height="700" alt="Speedup" src="https://github.com/user-attachments/assets/ab51f200-4b50-4df3-81c7-288c72e2f5ac" />

---

### 4. Eficiência

Representa o percentual de aproveitamento do hardware, calculado pela razão entre o **speedup** e o número de threads utilizadas (**8**).

<img width="940" height="700" alt="Eficiência" src="https://github.com/user-attachments/assets/064abe44-81f1-4108-b565-6f5a84d7574f" />
