#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>

#define CONJUNTO "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define TAM_CONJUNTO (sizeof(CONJUNTO) - 1)
#define TAM_MAX_SENHA 100

char *senha_alvo;
int tam_senha;
atomic_int achou = 0;

void *forca_bruta(void *arg)
{
    int id_thread = ((int *)arg)[0];
    int qtd_threads = ((int *)arg)[1];
    free(arg);

    long long total_combinacoes = 1;
    for (int i = 0; i < tam_senha; i++)
        total_combinacoes *= TAM_CONJUNTO;

    char tentativa[TAM_MAX_SENHA + 1];

    for (long long i = id_thread; i < total_combinacoes && !achou; i += qtd_threads)
    {
        long long indice = i;
        for (int j = 0; j < tam_senha; j++)
        {
            tentativa[j] = CONJUNTO[indice % TAM_CONJUNTO];
            indice /= TAM_CONJUNTO;
        }
        tentativa[tam_senha] = '\0';

        if (memcmp(tentativa, senha_alvo, tam_senha) == 0)
        {
            if (!achou)
            {
                achou = 1;
                printf("Senha encontrada: %s\n", tentativa);
            }
            break;
        }
    }

    return NULL;
}

void medir_tempo(int qtd_threads)
{
    clock_t inicio, fim;
    double tempo_gasto;

    pthread_t *threads = (pthread_t *)malloc(qtd_threads * sizeof(pthread_t));

    inicio = clock();

    for (int i = 0; i < qtd_threads; i++)
    {
        int *arg = (int *)malloc(sizeof(int) * 2);
        arg[0] = i;
        arg[1] = qtd_threads;

        if (pthread_create(&threads[i], NULL, forca_bruta, arg) != 0)
        {
            perror("Erro ao criar thread");
            return;
        }
    }

    for (int i = 0; i < qtd_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    fim = clock();
    tempo_gasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo com %d threads: %.2f segundos\n", qtd_threads, tempo_gasto);

    free(threads);
}

int main()
{
    int qtd_threads;

    printf("Digite a senha: ");
    senha_alvo = (char *)malloc(TAM_MAX_SENHA * sizeof(char));
    fgets(senha_alvo, TAM_MAX_SENHA, stdin);
    senha_alvo[strcspn(senha_alvo, "\n")] = '\0';

    tam_senha = strlen(senha_alvo);

    for (qtd_threads = 1; qtd_threads <= 8; qtd_threads++)
    {
        achou = 0;
        medir_tempo(qtd_threads);
    }

    free(senha_alvo);
    return 0;
}
