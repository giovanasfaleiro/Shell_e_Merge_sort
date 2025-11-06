#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para memcpy
#include <time.h>   // Para medir o tempo
#include <math.h>   // Para pow() (usado em Knuth)

// Estrutura para armazenar as métricas da atividade
 
typedef struct {
    long long comparacoes;
    long long movimentacoes;
} Stats;


//Gera os dados para os testes. 
void gerarArray(int arr[], int n, int tipo) {
    srand(time(NULL)); 
    for (int i = 0; i < n; i++) {
        switch (tipo) {
            case 0: // Aleatório
                arr[i] = rand() % (n * 10);
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

void copiarArray(int original[], int copia[], int n) {
    memcpy(copia, original, n * sizeof(int));
}


// Implementação do Shellsort

Stats shellSort(int arr[], int n, int gaps[], int num_gaps) {
    Stats stats = {0, 0};

    // Itera sobre a sequência de gaps, DO MAIOR PARA O MENOR
    for (int g_idx = num_gaps - 1; g_idx >= 0; g_idx--) {
        int gap = gaps[g_idx]; // Pega o 'salto' atual

        //começa do primeiro elemento que tem alguém 'gap' posições atrás
        for (int i = gap; i < n; i++) {
            
            // pega o elemento para inserir 
            int temp = arr[i];
            stats.movimentacoes++; 
            
            int j = i;

            // move os elementos maiores para a frente
            
            while (j >= gap && (stats.comparacoes++, arr[j - gap] > temp)) {
                
                // Desloca o elemento maior para a frente
                arr[j] = arr[j - gap];
                stats.movimentacoes++; 
                
                j -= gap; // Pula para a próxima posição
            }
            
            // Encaixa o 'temp' no buraco que abrimos
            arr[j] = temp;
            stats.movimentacoes++; 
        }
    }
    return stats;
}


// Funções Geradoras de Gaps 

int* gerarGapsShell(int n, int *num_gaps) {
    // Aloca um array temporário (grande o suficiente)
    int *gaps_temp = (int*)malloc(n * sizeof(int));
    int count = 0;
    
    // Gera a sequência de Shell
    for (int gap = n / 2; gap > 0; gap /= 2) {
        gaps_temp[count++] = gap;
    }
    
    // Aloca o array final com o tamanho exato
    int *gaps = (int*)malloc(count * sizeof(int));
    
    // Inverte a ordem 
    for (int i = 0; i < count; i++) {
        gaps[i] = gaps_temp[count - 1 - i];
    }

    *num_gaps = count;
    free(gaps_temp);
    return gaps;
}

//Gera a sequência de Knuth
int* gerarGapsKnuth(int n, int *num_gaps) {
    // Aloca um array temporário (50 é mais que suficiente)
    int *gaps_temp = (int*)malloc(50 * sizeof(int));
    int count = 0;
    long gap = 1;
    int k = 1;
    
    // Gera gaps enquanto forem menores que o tamanho do array
    while (gap < n) {
        gaps_temp[count++] = (int)gap;
        k++;
        gap = (pow(3, k) - 1) / 2; // (3^k - 1) / 2
    }
    
    // Copia para um array do tamanho exato
    int *gaps = (int*)malloc(count * sizeof(int));
    memcpy(gaps, gaps_temp, count * sizeof(int));
    
    *num_gaps = count;
    free(gaps_temp);
    return gaps;
}

// Gera a sequência de Ciura

int* gerarGapsCiura(int n, int *num_gaps) {
    int gaps_base[] = {1, 4, 10, 23, 57, 132, 301, 701, 1750};
    int base_count = sizeof(gaps_base) / sizeof(gaps_base[0]);
    
    // Aloca array temporário
    int *gaps_temp = (int*)malloc(50 * sizeof(int));
    memcpy(gaps_temp, gaps_base, base_count * sizeof(int));
    
    int count = base_count;
 
    while (gaps_temp[count - 1] < n) {
        long next_gap = (long)(gaps_temp[count - 1] * 2.2);
        
        // Não adiciona um gap que seja maior ou igual a N
        if (next_gap >= n) break; 
        
        gaps_temp[count] = (int)next_gap;
        count++;
    }

    // Copia para um array do tamanho exato
    int *gaps = (int*)malloc(count * sizeof(int));
    memcpy(gaps, gaps_temp, count * sizeof(int));

    *num_gaps = count;
    free(gaps_temp);
    return gaps;
}




int main() {
    int tamanhos[] = {1000, 10000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    for (int i = 0; i < num_tamanhos; i++) {
        int n = tamanhos[i];
        printf("--- Testando Shellsort para N = %d ---\n\n", n);

        int *arr_original = (int*)malloc(n * sizeof(int));
        int *arr_copia = (int*)malloc(n * sizeof(int));

        // Testa os 3 cenários
        for (int cenario = 0; cenario < 3; cenario++) {
            gerarArray(arr_original, n, cenario);
            
            const char* nomes_cenarios[] = {"Aleatorio", "Ordenado", "Inverso"};
            printf("### Cenario: %s ###\n", nomes_cenarios[cenario]);

            // --- Teste 1: Gaps de Shell ---
            int num_gaps_shell;
            int *gaps_shell = gerarGapsShell(n, &num_gaps_shell);
            copiarArray(arr_original, arr_copia, n);
            
            clock_t inicio_s = clock();
            Stats stats_s = shellSort(arr_copia, n, gaps_shell, num_gaps_shell);
            clock_t fim_s = clock();
            double tempo_s = ((double)(fim_s - inicio_s)) / CLOCKS_PER_SEC * 1000.0;

            printf("Shellsort (Shell):\n");
            printf("  Tempo: %.2f ms\n", tempo_s);
            printf("  Comparacoes: %lld\n", stats_s.comparacoes);
            printf("  Movimentacoes: %lld\n\n", stats_s.movimentacoes);
            free(gaps_shell); // Libera o array de gaps

            // --- Teste 2: Gaps de Knuth ---
            int num_gaps_knuth;
            int *gaps_knuth = gerarGapsKnuth(n, &num_gaps_knuth);
            copiarArray(arr_original, arr_copia, n);

            clock_t inicio_k = clock();
            Stats stats_k = shellSort(arr_copia, n, gaps_knuth, num_gaps_knuth);
            clock_t fim_k = clock();
            double tempo_k = ((double)(fim_k - inicio_k)) / CLOCKS_PER_SEC * 1000.0;
            
            printf("Shellsort (Knuth):\n");
            printf("  Tempo: %.2f ms\n", tempo_k);
            printf("  Comparacoes: %lld\n", stats_k.comparacoes);
            printf("  Movimentacoes: %lld\n\n", stats_k.movimentacoes);
            free(gaps_knuth); // Libera o array de gaps

            // --- Teste 3: Gaps de Ciura ---
            int num_gaps_ciura;
            int *gaps_ciura = gerarGapsCiura(n, &num_gaps_ciura);
            copiarArray(arr_original, arr_copia, n);

            clock_t inicio_c = clock();
            Stats stats_c = shellSort(arr_copia, n, gaps_ciura, num_gaps_ciura);
            clock_t fim_c = clock();
            double tempo_c = ((double)(fim_c - inicio_c)) / CLOCKS_PER_SEC * 1000.0;

            printf("Shellsort (Ciura):\n");
            printf("  Tempo: %.2f ms\n", tempo_c);
            printf("  Comparacoes: %lld\n", stats_c.comparacoes);
            printf("  Movimentacoes: %lld\n\n", stats_c.movimentacoes);
            free(gaps_ciura); // Libera o array de gaps
        }
        
        free(arr_original);
        free(arr_copia);
        printf("--------------------------------\n\n");
    }

    return 0;
}