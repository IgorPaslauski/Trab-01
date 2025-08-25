#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include <ctype.h>

#define CONJUNTO_NUMERICOS "0123456789"
#define CONJUNTO_COMPLETO "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define CONJUNTO_ALFABETO "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define TAM_MAX_SENHA 100

char *senha_alvo;
int tam_senha;
atomic_int achou = 0;
atomic_ullong tentativas = 0;
const char *conjunto_atual;
int tam_conjunto;

void definir_charset()
{
    int tem_alfabeto = 0;
    int tem_numero = 0;

    for (int i = 0; i < tam_senha; i++)
    {
        if (isdigit((unsigned char)senha_alvo[i]))
            tem_numero = 1;
        else
            tem_alfabeto = 1;
    }

    if (tem_alfabeto && tem_numero)
        conjunto_atual = CONJUNTO_COMPLETO;
    else if (tem_numero)
        conjunto_atual = CONJUNTO_NUMERICOS;
    else
        conjunto_atual = CONJUNTO_ALFABETO;

    tam_conjunto = strlen(conjunto_atual);
}

void *forca_bruta(void *arg)
{
    int *args = (int *)arg;
    int id_thread = args[0];
    int qtd_threads = args[1];
    free(arg);

    unsigned long long total_combinacoes = 1;
    for (int i = 0; i < tam_senha; i++)
        total_combinacoes *= tam_conjunto;

    char tentativa[TAM_MAX_SENHA + 1];

    for (unsigned long long i = id_thread; i < total_combinacoes && !achou; i += qtd_threads)
    {
        unsigned long long indice = i;
        for (int j = 0; j < tam_senha; j++)
        {
            tentativa[j] = conjunto_atual[indice % tam_conjunto];
            indice /= tam_conjunto;
        }
        tentativa[tam_senha] = '\0';

        atomic_fetch_add(&tentativas, 1);

        if (memcmp(tentativa, senha_alvo, tam_senha) == 0)
        {
            if (atomic_exchange(&achou, 1) == 0)
            {
                printf("Senha encontrada: %s\n", tentativa);
                printf("Tentativas realizadas: %llu\n", (unsigned long long)tentativas);
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

    pthread_t *threads = malloc(qtd_threads * sizeof(pthread_t));

    inicio = clock();

    for (int i = 0; i < qtd_threads; i++)
    {
        int *arg = malloc(2 * sizeof(int));
        arg[0] = i;
        arg[1] = qtd_threads;

        if (pthread_create(&threads[i], NULL, forca_bruta, arg) != 0)
        {
            perror("Erro ao criar thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < qtd_threads; i++)
        pthread_join(threads[i], NULL);

    fim = clock();
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Tempo com %d threads: %.2f segundos\n", qtd_threads, tempo_gasto);

    free(threads);
}

int main()
{
    senha_alvo = malloc(TAM_MAX_SENHA * sizeof(char));

    printf("Digite a senha: ");
    if (!fgets(senha_alvo, TAM_MAX_SENHA, stdin))
    {
        perror("Erro ao ler senha");
        return EXIT_FAILURE;
    }
    senha_alvo[strcspn(senha_alvo, "\n")] = '\0';

    tam_senha = strlen(senha_alvo);
    definir_charset();

    printf("Charset escolhido: %s\n", conjunto_atual);

    for (int qtd_threads = 1; qtd_threads <= 8; qtd_threads++)
    {
        achou = 0;
        tentativas = 0;
        medir_tempo(qtd_threads);
    }

    free(senha_alvo);
    return 0;
}
