#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define CONJUNTO_NUMERICOS "0123456789"
#define CONJUNTO_COMPLETO "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define CONJUNTO_ALFABETO "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_+-=[]{}|;:,.<>?"
#define TAM_MAX_SENHA 100

char *senha_alvo;
int tam_senha;
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

void forca_bruta()
{
    char tentativa[TAM_MAX_SENHA + 1];

    unsigned long long total_combinacoes = 1;
    for (int i = 0; i < tam_senha; i++)
        total_combinacoes *= tam_conjunto;

    unsigned long long contador_local = 0;

    for (unsigned long long i = 0; i < total_combinacoes; i++)
    {
        unsigned long long indice = i;
        for (int j = 0; j < tam_senha; j++)
        {
            tentativa[j] = conjunto_atual[indice % tam_conjunto];
            indice /= tam_conjunto;
        }
        tentativa[tam_senha] = '\0';

        contador_local++;

        if (memcmp(tentativa, senha_alvo, tam_senha) == 0)
        {
            printf("Senha encontrada: %s\n", tentativa);
            printf("Tentativas realizadas: %llu\n", (unsigned long long)contador_local);
            break;
        }
    }
}

void medir_tempo()
{
    clock_t inicio, fim;
    double tempo_gasto;

    inicio = clock();

    forca_bruta();

    fim = clock();
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Tempo com 1 thread: %.2f segundos\n", tempo_gasto);
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

    medir_tempo();

    free(senha_alvo);
    return 0;
}
