

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cidades.h"

int main()
{
    Estrada *estrada;

    estrada = getEstrada("cidades.txt");
    if (estrada == NULL)
    {
        printf("Falha ao alocar estrada\n");
        return 1;
    }

    printf("%d\n%d\n", estrada->T, estrada->N);
    for (int i = 0; i < estrada->N; i++)
    {
        printf("%d %s\n", estrada->C[i].Posicao, estrada->C[i].Nome);
    }

    free(estrada->C);
    free(estrada);
    return 0;
}

Estrada *getEstrada(const char *nomeArquivo)
{
    Estrada *Eaux;
    Cidade Caux;
    int TamanhoDaEstrada, NumDeCidades;

    FILE *Arquivo = fopen(nomeArquivo, "r");
    if (Arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    fscanf(Arquivo, "%d\n%d\n", &TamanhoDaEstrada, &NumDeCidades);

    if (checarErroGetEstrada(TamanhoDaEstrada, NumDeCidades) == NULL)
    {
        fclose(Arquivo);
        return NULL;
    }

    if (checarNumeroDeCidades(NumDeCidades, nomeArquivo) == NULL)
    {
        fclose(Arquivo);
        return NULL;
    }

    Eaux = (Estrada *)malloc(sizeof(Estrada));
    if (Eaux == NULL)
    {
        printf("Erro ao alocar memória para estrada.\n");
        fclose(Arquivo);
        return NULL;
    }

    Eaux->T = TamanhoDaEstrada;
    Eaux->N = NumDeCidades;
    Eaux->C = (Cidade *)malloc(sizeof(Cidade) * NumDeCidades);
    if (Eaux->C == NULL)
    {
        printf("Erro ao alocar memória para cidades.\n");
        free(Eaux);
        fclose(Arquivo);
        return NULL;
    }

    rewind(Arquivo);
    fscanf(Arquivo, "%d\n%d\n", &TamanhoDaEstrada, &NumDeCidades);

    for (int i = 0; i < NumDeCidades; i++)
    {
        if (fscanf(Arquivo, "%d %[^\n]\n", &Caux.Posicao, Caux.Nome) != 2)
        {
            printf("Erro ao ler cidade %d.\n", i + 1);
            free(Eaux->C);
            free(Eaux);
            fclose(Arquivo);
            return NULL;
        }

        if (checarErroGetCidade(Caux.Posicao, i, TamanhoDaEstrada, NumDeCidades) == NULL)
        {
            printf("Erro ao alocar cidades.\n");
            free(Eaux->C);
            free(Eaux);
            fclose(Arquivo);
            return NULL;
        }

        Eaux->C[i] = Caux;

        for (int j = 0; j < i; j++)
        {
            if (Eaux->C[i].Posicao == Eaux->C[j].Posicao)
            {
                printf("Erro ao alocar cidades: Os pares nas linhas \"%d\" e \"%d\" tem o mesmo valor.\n", j + 3, i + 3);
                free(Eaux->C);
                free(Eaux);
                fclose(Arquivo);
                return NULL;
            }
        }
    }

    fclose(Arquivo);
    return Eaux;
}

char *checarErroGetCidade(int posicao, int linha, int TamanhoDaEstrada, int NumDeCidades)
{
    char *noError = "sem erro";

    if (posicao < 0 || posicao > TamanhoDaEstrada)
    {
        printf("Posicao \"%d\" nao e uma posicao valida\n", posicao);
        return NULL;
    }

    if (linha >= NumDeCidades)
    {
        printf("A cidade na linha \"%d\" nao e uma cidade valida\n", linha);
        return NULL;
    }

    return noError;
}

char *checarErroGetEstrada(int TamanhoDaEstrada, int NumDeCidades)
{
    char *noError = "sem erro";

    if (TamanhoDaEstrada < 3 || TamanhoDaEstrada > 1000000)
    {
        printf("Tamanho de estrada invalido\n");
        return NULL;
    }

    if (NumDeCidades < 2 || NumDeCidades > 10000)
    {
        printf("Numero de cidades invalido\n");
        return NULL;
    }

    return noError;
}

char *checarNumeroDeCidades(int NumeroDeCidades, const char *nomeDoArquivo)
{
    char *NoError = "sem erro";
    int contador = 0;
    char linha[256];

    FILE *Arquivo = fopen(nomeDoArquivo, "r");
    if (Arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    while (fgets(linha, sizeof(linha), Arquivo) != NULL)
    {
        contador++;
    }

    fclose(Arquivo);

    if (NumeroDeCidades != contador - 2)
    {
        printf("O numero de cidades especificado na linha 2 nao condiz com o numero de cidades especificadas no resto do arquivo.\n");
        return NULL;
    }

    return NoError;
}