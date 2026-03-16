#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define Arquivo_Entrada "animacao.tex"
#define Arquivo_Saida "animacao.huff"
#define MAGIC "HUF1"

typedef struct No {
    unsigned char simbolo;
    uint64_t frequencia;
    int folha;
    struct No *esq, *dir;
} No;

typedef struct {
    No **data;
    int size, cap;
} MinHeap;

MinHeap* criar_heap(int cap) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->data = malloc(sizeof(No*) * cap);
    h->size = 0; h->cap = cap;
    return h;
}
void inserir_heap(MinHeap *h, No *n) {
    if (h->size >= h->cap) {
        h->cap *= 2;
        h->data = realloc(h->data, sizeof(No*) * h->cap);
    }
    int i = h->size++;
    while (i > 0) {
        int p = (i-1)/2;
        if (h->data[p]->frequencia <= n->frequencia) break;
        h->data[i] = h->data[p]; i = p;
    }
    h->data[i] = n;
}
No* remover_heap(MinHeap *h) {
    if (h->size == 0) return NULL;
    No *res = h->data[0];
    No *last = h->data[--h->size];
    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, smallest = i;
        if (l < h->size && h->data[l]->frequencia < last->frequencia) smallest = l;
        if (r < h->size && h->data[r]->frequencia < h->data[smallest]->frequencia) smallest = r;
        if (smallest == i) break;
        h->data[i] = h->data[smallest];
        i = smallest;
    }
    h->data[i] = last;
    return res;
}

No* criar_folha(unsigned char simbolo, uint64_t frequencia) {
    No *n = malloc(sizeof(No));
    n->simbolo = simbolo; n->frequencia = frequencia; n->folha = 1;
    n->esq = n->dir = NULL;
    return n;
}
No* criar_no(No *l, No *r) {
    No *n = malloc(sizeof(No));
    n->folha = 0; n->esq = l; n->dir = r;
    n->frequencia = l->frequencia + r->frequencia;
    return n;
}

void gerarCodigos(No *raiz, char **tabelaCodigos, char *buffer, int profundidade) {
    if (!raiz) return;
    if (raiz->folha) {
        if (profundidade == 0) { buffer[profundidade++] = '0'; } // single simbolobol
        buffer[profundidade] = '\0';
        tabelaCodigos[raiz->simbolo] = malloc(profundidade+1);
        strcpy(tabelaCodigos[raiz->simbolo], buffer);
        return;
    }
    buffer[profundidade] = '0'; gerarCodigos(raiz->esq, tabelaCodigos, buffer, profundidade+1);
    buffer[profundidade] = '1'; gerarCodigos(raiz->dir, tabelaCodigos, buffer, profundidade+1);
}

void serializar_arv(No *No, unsigned char **out, size_t *out_len, size_t *out_cap) {
    if (!No) return;
    if (No->folha) {
        if (*out_len + 2 > *out_cap) {
            *out_cap = (*out_cap == 0) ? 256 : (*out_cap * 2);
            *out = realloc(*out, *out_cap);
        }
        (*out)[(*out_len)++] = 1;
        (*out)[(*out_len)++] = No->simbolo;
    } else {
        if (*out_len + 1 > *out_cap) {
            *out_cap = (*out_cap == 0) ? 256 : (*out_cap * 2);
            *out = realloc(*out, *out_cap);
        }
        (*out)[(*out_len)++] = 0;
        serializar_arv(No->esq, out, out_len, out_cap);
        serializar_arv(No->dir, out, out_len, out_cap);
    }
}

typedef struct {
    FILE *f;
    unsigned char buffer;
    int bitcount;
} BitWriter;
BitWriter* bw_create(FILE *f) {
    BitWriter *bw = malloc(sizeof(BitWriter));
    bw->f = f; bw->buffer = 0; bw->bitcount = 0;
    return bw;
}
void bw_write_bit(BitWriter *bw, int bit) {
    bw->buffer = (bw->buffer << 1) | (bit & 1);
    bw->bitcount++;
    if (bw->bitcount == 8) {
        fputc(bw->buffer, bw->f);
        bw->buffer = 0; bw->bitcount = 0;
    }
}
void bw_write_bits_from_string(BitWriter *bw, const char *s) {
    for (int i=0; s[i]; ++i) bw_write_bit(bw, s[i]=='1');
}
int bw_flush(BitWriter *bw) {
    int valid_bits = 8;
    if (bw->bitcount != 0) {
        int shift = 8 - bw->bitcount;
        bw->buffer <<= shift;
        fputc(bw->buffer, bw->f);
        valid_bits = bw->bitcount;
    }
    return valid_bits;
}

int main(void) {
    FILE *fin = fopen(Arquivo_Entrada, "rb");
    if (!fin) {
        fprintf(stderr, "Erro: nao abriu %s\n", Arquivo_Entrada);
        return 1;
    }
    fseek(fin, 0, SEEK_END);
    uint64_t filesize = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    unsigned char *data = malloc(filesize ? filesize : 1);
    if (filesize) fread(data, 1, filesize, fin);
    fclose(fin);

    uint64_t frequencia[256] = {0};
    for (uint64_t i=0;i<filesize;i++) frequencia[data[i]]++;

    MinHeap *h = criar_heap(256);
    for (int i=0;i<256;i++) if (frequencia[i]>0) inserir_heap(h, criar_folha((unsigned char)i, frequencia[i]));
    if (h->size==0) {
        FILE *fout = fopen(Arquivo_Saida,"wb");
        fwrite(MAGIC,1,4,fout);
        uint64_t z = 0; fwrite(&z, sizeof(uint64_t), 1, fout);
        uint32_t tsize = 0; fwrite(&tsize, sizeof(uint32_t), 1, fout);
        unsigned char lastbits = 0; fwrite(&lastbits,1,1,fout);
        fclose(fout);
        printf("Compressao concluida.\n");
        free(data); free(h->data); free(h);
        return 0;
    }

    No *raiz = NULL;
    if (h->size == 1) raiz = remover_heap(h);
    else {
        while (h->size > 1) {
            No *a = remover_heap(h), *b = remover_heap(h);
            No *p = criar_no(a,b);
            inserir_heap(h, p);
        }
        raiz = remover_heap(h);
    }

    char *tabelaCodigos[256] = {0};
    char tempbuffer[512];
    gerarCodigos(raiz, tabelaCodigos, tempbuffer, 0);

    unsigned char *treebuffer = NULL; size_t treelen = 0, treecap = 0;
    serializar_arv(raiz, &treebuffer, &treelen, &treecap);

    FILE *fout = fopen(Arquivo_Saida,"wb");
    if (!fout) { perror("fopen output"); return 3; }
    fwrite(MAGIC,1,4,fout);
    fwrite(&filesize, sizeof(uint64_t), 1, fout);
    uint32_t treelen32 = (uint32_t)treelen;
    fwrite(&treelen32, sizeof(uint32_t), 1, fout);
    if (treelen) fwrite(treebuffer, 1, treelen, fout);
    long pos_lastbits = ftell(fout);
    unsigned char placeholder = 0;
    fwrite(&placeholder,1,1,fout);

    BitWriter *bw = bw_create(fout);
    for (uint64_t i=0;i<filesize;i++) {
        const char *c = tabelaCodigos[data[i]];
        bw_write_bits_from_string(bw, c);
    }
    int valid_bits_in_last = bw_flush(bw);
    fseek(fout, pos_lastbits, SEEK_SET);
    unsigned char vb = (unsigned char)valid_bits_in_last;
    fwrite(&vb,1,1,fout);
    fclose(fout);

    printf("Compressao concluida.\n");

FILE *fcomp = fopen(Arquivo_Saida, "rb");
fseek(fcomp, 0, SEEK_END);
long tamanho_comprimido = ftell(fcomp);
fclose(fcomp);
double porcentagem = (1.0 - ((double)tamanho_comprimido / (double)filesize)) * 100.0;
printf("Tamanho original: %llu bytes\n", (unsigned long long)filesize);
printf("Tamanho comprimido: %ld bytes\n", tamanho_comprimido);
printf("Compressao: %.2f%%\n", porcentagem);


    for (int i=0;i<256;i++) if (tabelaCodigos[i]) free(tabelaCodigos[i]);
    free(data); free(treebuffer); free(h->data); free(h);
    return 0;
}
