#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define Arquivo_Entrada "animacao.huff"
#define Arquivo_Saida "animacao.tex"
#define MAGIC "HUF1"

typedef struct No {
    unsigned char simbolo;
    int Folha;
    struct No *esq, *dir;
} No;

No* criar_no_interno() {
    No *semAtual = malloc(sizeof(No));
    semAtual->Folha = 0; semAtual->esq = semAtual->dir = NULL;
    return semAtual;
}
No* criar_folha(unsigned char s) {
    No *semAtual = malloc(sizeof(No));
    semAtual->Folha = 1; semAtual->simbolo = s; semAtual->esq = semAtual->dir = NULL;
    return semAtual;
}

No* desserializar_arv(const unsigned char *buffer, size_t tamanho, size_t *indice) {
    if (*indice >= tamanho) return NULL;
    unsigned char marcador = buffer[(*indice)++];
    if (marcador == 1) {
        unsigned char simbolo = buffer[(*indice)++];
        return criar_folha(simbolo);
    } else {
        No *semAtual = criar_no_interno();
        semAtual->esq = desserializar_arv(buffer, tamanho, indice);
        semAtual->dir = desserializar_arv(buffer, tamanho, indice);
        return semAtual;
    }
}

int main(void) {
    FILE *arquivoEntrada = fopen(Arquivo_Entrada,"rb");
    if (!arquivoEntrada) { fprintf(stderr, "Erro: nao abriu %s\n", Arquivo_Entrada); return 2; }

    char magic[5] = {0};
    fread(magic,1,4,arquivoEntrada);
    if (strncmp(magic, MAGIC, 4) != 0) {
        fprintf(stderr, "Arquivo nao eh .huff (magic mismatch)\n");
        fclose(arquivoEntrada); return 3;
    }
    uint64_t tamanho_original = 0;
    fread(&tamanho_original, sizeof(uint64_t), 1, arquivoEntrada);
    uint32_t tamanhoArvore = 0;
    fread(&tamanhoArvore, sizeof(uint32_t), 1, arquivoEntrada);
    unsigned char *bufferArvore = NULL;
    if (tamanhoArvore) {
        bufferArvore = malloc(tamanhoArvore);
        fread(bufferArvore,1,tamanhoArvore,arquivoEntrada);
    }
    unsigned char valid_bits_last = 0;
    fread(&valid_bits_last,1,1,arquivoEntrada);

    No *raiz = NULL;
    if (tamanhoArvore) {
        size_t indice = 0;
        raiz = desserializar_arv(bufferArvore, tamanhoArvore, &indice);
    } else {
        FILE *arquivoSaida = fopen(Arquivo_Saida,"wb");
        fclose(arquivoSaida);
        printf("Descompressao concluida.\n");
        fclose(arquivoEntrada);
        return 0;
    }

    FILE *arquivoSaida = fopen(Arquivo_Saida,"wb");
    if (!arquivoSaida) { perror("fopen output"); return 4; }

    fseek(arquivoEntrada, 0, SEEK_END);
    long endpos = ftell(arquivoEntrada);
    long data_start = 4 + 8 + 4 + tamanhoArvore + 1;
    long datasz = endpos - data_start;
    fseek(arquivoEntrada, data_start, SEEK_SET);

    long bytes_read = 0;
    unsigned char atual = 0;
    No *cura = raiz;
    uint64_t bytesEscritos = 0;

    while (bytes_read < datasz && bytesEscritos < tamanho_original) {
        if (fread(&atual,1,1,arquivoEntrada) != 1) break;
        bytes_read++;
        int bits_to_process = 8;
        if (bytes_read == datasz) { bits_to_process = valid_bits_last ? valid_bits_last : 8; }
        for (int b = 7; b >= 8 - bits_to_process; --b) {
            int bit = (atual >> b) & 1;
            if (bit) cura = cura->dir; else cura = cura->esq;
            if (!cura) { fprintf(stderr, "Erro na decodificacao (arvore invalida)\n"); fclose(arquivoEntrada); fclose(arquivoSaida); return 5; }
            if (cura->Folha) {
                fputc(cura->simbolo, arquivoSaida);
                bytesEscritos++;
                if (bytesEscritos >= tamanho_original) break;
                cura = raiz;
            }
        }
    }

    fclose(arquivoSaida);
    fclose(arquivoEntrada);

    FILE *frest = fopen(Arquivo_Saida, "rb");
fseek(frest, 0, SEEK_END);
long tamanho_restaurado = ftell(frest);
fclose(frest);

double porcentagem_restaurada = ((double)tamanho_restaurado / (double)tamanho_original) * 100.0;

printf("Restauracao: %.2f%%\n", porcentagem_restaurada);


    printf("Descompressao concluida.\n");
    return 0;
}
