#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cidades.h"

char* checarErroGetEstrada(int TamanhoDaEstrada, int NumDeCidades);
char* checarErroGetCidade(int posicao, int linha, int TamanhoDaEstrada, int NumDeCidades);
char* checarErroNumeroDeCidades(int NumeroDeCidades, const char *nomeDoArquivo);
void polirNomes(Estrada *estrada);
void organizarEstrada(Estrada *estrada);


Estrada *getEstrada(const char *nomeArquivo) {
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

    if (checarErroNumeroDeCidades(NumDeCidades, nomeArquivo) == NULL)
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

    // Tecnicamente apartir daqui e um getCidade
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

    polirNomes(Eaux);
    organizarEstrada(Eaux);

    fclose(Arquivo);
    return Eaux;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {

    Estrada *estrada = getEstrada(nomeArquivo);

    double Vizinhanca[2][estrada->N]; // vizinhanca[0] guarda a posicao e vizinhanca[1] quantos km a cidade tem que manter

    for(int i  = 0; i < estrada->N; i++) {

        if(i == 0) { 
            Vizinhanca[0][i] = (double) estrada->C[0].Posicao;
            Vizinhanca[1][i] = (double) estrada->C[0].Posicao + (estrada->C[1].Posicao - estrada->C[0].Posicao) / 2.0;
            continue;
        }
        
        if(i == estrada->N - 1) {
            Vizinhanca[0][i] = (double) estrada->C[i].Posicao;
            Vizinhanca[1][i] = (double) ((estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0) + (estrada->T - estrada->C[i].Posicao);
            continue;
        }
        
        Vizinhanca[0][i] = (double) estrada->C[i].Posicao;
        Vizinhanca[1][i] = (double) ((estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0) + ((estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0);
    }


    double MenorVizinhanca[2];
    MenorVizinhanca[0] = Vizinhanca[0][0];
    MenorVizinhanca[1] = Vizinhanca[1][0];

    for (int i = 0; i < estrada->N; i++)
    {

        if (Vizinhanca[1][i] < MenorVizinhanca[1])
        {

            MenorVizinhanca[0] = Vizinhanca[0][i];
            MenorVizinhanca[1] = Vizinhanca[1][i];

        }
    }

    return MenorVizinhanca[1];

    
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {

    Estrada *estrada = getEstrada(nomeArquivo);

    double Vizinhanca[2][estrada->N]; // vizinhanca[0] guarda a posicao e vizinhanca[1] quantos km a cidade tem que manter

    for(int i  = 0; i < estrada->N; i++) {

        if(i == 0) { 
            Vizinhanca[0][i] = (double) estrada->C[0].Posicao;
            Vizinhanca[1][i] = (double) estrada->C[0].Posicao + (estrada->C[1].Posicao - estrada->C[0].Posicao) / 2.0;
            continue;
        }
        
        if(i == estrada->N - 1) {
            Vizinhanca[0][i] = (double) estrada->C[i].Posicao;
            Vizinhanca[1][i] = (double) ((estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0) + (estrada->T - estrada->C[i].Posicao);
            continue;
        }
        
        Vizinhanca[0][i] = (double) estrada->C[i].Posicao;
        Vizinhanca[1][i] = (double) ((estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0) + ((estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0);
    }


    double MenorVizinhanca[2];
    MenorVizinhanca[0] = Vizinhanca[0][0];
    MenorVizinhanca[1] = Vizinhanca[1][0];

    for (int i = 0; i < estrada->N; i++)
    {

        if (Vizinhanca[1][i] < MenorVizinhanca[1])
        {

            MenorVizinhanca[0] = Vizinhanca[0][i];
            MenorVizinhanca[1] = Vizinhanca[1][i];

        }
    }

    int i = 0;
    while(1) {
        if (MenorVizinhanca[0] == estrada->C[i].Posicao) {return estrada->C[i].Nome; }
        i++;
    }
    
}

char *checarErroGetCidade(int posicao, int linha, int TamanhoDaEstrada, int NumDeCidades) {
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

char *checarErroGetEstrada(int TamanhoDaEstrada, int NumDeCidades) {
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

char *checarErroNumeroDeCidades(int NumeroDeCidades, const char *nomeDoArquivo) {
    
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

void organizarEstrada(Estrada *estrada) {
    Cidade Caux;
    for (int i = 0; i < estrada->N - 1; i++)
    {
        for (int j = 0; j < estrada->N - 1 - i; j++)
        {
            if (estrada->C[j].Posicao > estrada->C[j + 1].Posicao)
            {
                Caux = estrada->C[j];
                estrada->C[j] = estrada->C[j + 1];
                estrada->C[j + 1] = Caux;
            }
        }
    }
}

void polirNomes(Estrada *estrada) {

    for( int i = 0; i < estrada->N; i++) {
        int UltimoChar = strlen(estrada->C[i].Nome);
        if (UltimoChar > 0 && estrada->C[i].Nome[UltimoChar - 1] == ' ') {estrada->C[i].Nome[UltimoChar - 1] = '\0'; }
    }
    
    // Nos testes que eu fiz, as cidades tavam sendo armazenadas com um espaco no final (Exemplo: "Birnir Zana " e nao "Birnir Zana")
    // Essa funcao e so pra impedir isso de acontecer
    // (As strings estavam sendo armazenada com um espaco adicional pq eu copiei os exemplos do pdf que ele disponibilizou, e la tem espaco, mas eu nao sei como ele vai testar esse codigo ne) 
}
