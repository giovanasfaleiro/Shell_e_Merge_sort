#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para memcpy
#include <time.h>   // Para medir o tempo

// estrutura das métricas para comparação
typedef struct {
    long long comparacoes;
    long long movimentacoes;
} Stats;


// Gera os dados para os testes:

void gerarArray(int arr[], int n, int tipo) {
    //geração de números aleatórios:
    srand(time(NULL)); 
    for (int i = 0; i < n; i++) {
        switch (tipo) {
            case 0: // Aleatório
                arr[i] = rand() % (n * 10); // Números de 0 a 10*N
                break;
            case 1: // Ordenado
                arr[i] = i;
                break;
            case 2: // Inverso
                arr[i] = n - 1 - i;
                break;
        }
    }
}

// Copia um array original para uma cópia(para testar cada algoritmo com os mesmos dados) 
void copiarArray(int original[], int copia[], int n) {
    // memcpy é a forma mais rápida de copiar um bloco de memória
    memcpy(copia, original, n * sizeof(int));
}


// Implementação do Mergesort 

void merge(int arr[], int l, int m, int r, Stats *stats) {
    int i, j, k;
    int n1 = m - l + 1; // Tamanho da pilha da Esquerda (L)
    int n2 = r - m;     // Tamanho da pilha da Direita (R)

    // Aloca memória temporária para as pilhas
    int *L = (int*)malloc(n1 * sizeof(int));
    int *R = (int*)malloc(n2 * sizeof(int));

    // Copia os dados do array principal para as pilhas temporárias
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
        stats->movimentacoes++; // Contando a cópia
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
        stats->movimentacoes++; // Contando a cópia
    }

    // Índices para percorrer as pilhas
    i = 0; // Índice da pilha L
    j = 0; // Índice da pilha R
    k = l; // Índice do array principal 'arr' (onde vamos escrever)

    // O loop principal de combinação
    while (i < n1 && j < n2) {
        stats->comparacoes++; 
        if (L[i] <= R[j]) {
            // Se L[i] é menor, ele vai para o array principal
            arr[k] = L[i];
            i++;
        } else {
            // Se R[j] é menor, ele vai para o array principal
            arr[k] = R[j];
            j++;
        }
        stats->movimentacoes++; 
        k++;
    }

    // Se a pilha R acabou, copia o que sobrou da pilha L
    while (i < n1) {
        arr[k] = L[i];
        stats->movimentacoes++; 
        i++;
        k++;
    }

    // Se a pilha L acabou, copia o que sobrou da pilha R
    while (j < n2) {
        arr[k] = R[j];
        stats->movimentacoes++; 
        j++;
        k++;
    }

    // Libera a memória temporária
    free(L);
    free(R);
}

//função para dividir
void mergeSortRecursive(int arr[], int l, int r, Stats *stats) {
    // Se 'l < r', significa que a pilha tem mais de 1 ítem
    if (l < r) {
        // Acha o meio
        int m = l + (r - l) / 2;

        // Chama a si mesma para a primeira metade (esquerda)
        mergeSortRecursive(arr, l, m, stats);

        // Chama a si mesma para a segunda metade (direita)
        mergeSortRecursive(arr, m + 1, r, stats);

        // Combina as duas metades que agora estão ordenadas
        merge(arr, l, m, r, stats);
    }
}

// chama a função recursiva.
 
Stats mergeSort(int arr[], int n) {
    Stats stats = {0, 0}; // Zera os contadores
    if (n > 0) {
        // Chama a função recursiva ("dividir")
        mergeSortRecursive(arr, 0, n - 1, &stats);
    }
    return stats;
}


int main() {
    // Define os tamanhos N para teste
    int tamanhos[] = {1000, 10000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    for (int i = 0; i < num_tamanhos; i++) {
        int n = tamanhos[i];
        printf("--- Testando Mergesort para N = %d ---\n\n", n);

        int *arr_original = (int*)malloc(n * sizeof(int));
        int *arr_copia = (int*)malloc(n * sizeof(int));

        // Testa os 3 cenários (aleatório, ordenado, inverso)
        for (int cenario = 0; cenario < 3; cenario++) {
            gerarArray(arr_original, n, cenario); 
            
            const char* nomes_cenarios[] = {"Aleatorio", "Ordenado", "Inverso"};
            printf("### Cenario: %s ###\n", nomes_cenarios[cenario]);

            copiarArray(arr_original, arr_copia, n);
            
            // Mede o tempo e executa a ordenação
            clock_t inicio = clock();
            
            Stats stats_merge = mergeSort(arr_copia, n); 
            
            clock_t fim = clock();
            
            // Calcula o tempo em milissegundos
            double tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC * 1000.0;
            
            printf("  Tempo: %.2f ms\n", tempo_gasto);
            printf("  Comparacoes: %lld\n", stats_merge.comparacoes);
            printf("  Movimentacoes: %lld\n\n", stats_merge.movimentacoes);
        }
        
        free(arr_original);
        free(arr_copia);
        printf("--------------------------------\n\n");
    }

    return 0;
}