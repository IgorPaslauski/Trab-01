#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define CHARSET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define CHARSET_SIZE (sizeof(CHARSET) - 1)
#define MAX_PASSWORD_LENGTH 100 // Senha pode ter até 100 caracteres

// Variáveis globais
char *target_password;
int target_len;
int found = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Função para gerar combinações
void *crack_password(void *arg)
{
    int thread_id = *((int *)arg);
    int num_threads = *((int *)(arg + sizeof(int)));
    int total_combinations = 1;
    for (int i = 0; i < target_len; i++)
    {
        total_combinations *= CHARSET_SIZE;
    }

    int start_index = (total_combinations / num_threads) * thread_id;
    int end_index = (total_combinations / num_threads) * (thread_id + 1);

    if (thread_id == num_threads - 1)
    {
        end_index = total_combinations;
    }

    // Gerar as combinações dentro do intervalo alocado para a thread
    char *attempt = (char *)malloc(target_len + 1);

    for (int i = start_index; i < end_index && !found; i++)
    {
        int index = i;
        for (int j = 0; j < target_len; j++)
        {
            attempt[j] = CHARSET[index % CHARSET_SIZE];
            index /= CHARSET_SIZE;
        }
        attempt[target_len] = '\0';

        // Verificar se a tentativa é igual à senha
        pthread_mutex_lock(&mutex);
        if (strcmp(attempt, target_password) == 0)
        {
            found = 1;
            printf("Senha encontrada: %s\n", attempt);
        }
        pthread_mutex_unlock(&mutex);
    }

    free(attempt);
    return NULL;
}

void measure_time(int num_threads)
{
    clock_t start, end;
    double cpu_time_used;

    // Cria as threads
    pthread_t *threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    int *thread_ids = (int *)malloc(num_threads * sizeof(int));

    // Inicia o cronômetro
    start = clock();

    // Cria as threads
    for (int i = 0; i < num_threads; i++)
    {
        thread_ids[i] = i;
        int *arg = (int *)malloc(sizeof(int) * 2);
        arg[0] = i;           // ID da thread
        arg[1] = num_threads; // Número de threads

        if (pthread_create(&threads[i], NULL, crack_password, arg) != 0)
        {
            perror("Erro ao criar thread");
            return;
        }
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Para o cronômetro
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Exibe o tempo que levou para quebrar a senha
    printf("Tempo para quebrar a senha com %d threads: %.2f segundos\n", num_threads, cpu_time_used);

    // Libera a memória alocada
    free(threads);
    free(thread_ids);
}

int main()
{
    int num_threads;

    // Solicita a senha a ser quebrada
    printf("Digite a senha a ser quebrada: ");
    target_password = (char *)malloc(MAX_PASSWORD_LENGTH * sizeof(char));
    fgets(target_password, MAX_PASSWORD_LENGTH, stdin);
    target_password[strcspn(target_password, "\n")] = '\0'; // Remove o '\n'

    target_len = strlen(target_password);

    // Medir o tempo com diferentes números de threads
    for (num_threads = 1; num_threads <= 8; num_threads++)
    {                              // Testa de 1 a 8 threads
        found = 0;                 // Reseta a flag de encontrado
        measure_time(num_threads); // Mede o tempo para cada número de threads
    }

    // Libera a memória da senha
    free(target_password);

    return 0;
}
