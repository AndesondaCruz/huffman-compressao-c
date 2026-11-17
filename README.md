# Huffman Compressão

Implementação do algoritmo de compressão de Huffman desenvolvida em linguagem C como projeto da disciplina de **Estruturas de Dados**.
O sistema permite **compressão e descompressão de arquivos utilizando codificação baseada em frequência de caracteres**, reduzindo o tamanho do arquivo original.

---

## Sobre o Projeto

A compressão de Huffman é um algoritmo clássico de compressão sem perdas que utiliza uma **árvore binária ponderada** para gerar códigos binários de tamanho variável para cada símbolo, de acordo com sua frequência de ocorrência.

Símbolos mais frequentes recebem códigos menores, enquanto símbolos menos frequentes recebem códigos maiores, resultando em uma redução do tamanho total do arquivo.

Este projeto implementa:

* Construção da **árvore de Huffman**
* Geração de **códigos binários prefix-free**
* Compressão de arquivos
* Descompressão do arquivo comprimido

---

## Tecnologias Utilizadas

* **C** – Linguagem principal do projeto
* **GCC** – Compilador utilizado
* **Linux / WSL** – Ambiente de desenvolvimento
* **Git** – Controle de versão

---

## Estrutura do Projeto

```
huffman-compressao-c
│
├── src
│   ├── compressao.c
│   └── descompressao.c
│
├── data
│   ├── animacao_original.tex
│   ├── animacao.tex
│   └── animacao.huff
│
├── bin              (gerada na compilação, fora do Git)
├── README.md
├── .gitignore
└── .gitattributes
```

**src/**
Contém os arquivos de código fonte responsáveis pela implementação da compressão e descompressão.

**data/**
Contém os arquivos utilizados para teste do algoritmo.

**bin/**
Recebe os executáveis gerados na compilação. Não é versionada, porque binários dependem do sistema de quem compila.

---

## Funcionamento do Algoritmo

O algoritmo segue as seguintes etapas:

1. Leitura do arquivo de entrada
2. Cálculo da frequência de cada caractere
3. Construção da **árvore de Huffman**
4. Geração dos códigos binários para cada símbolo
5. Compressão do arquivo utilizando os códigos gerados
6. Escrita do arquivo comprimido

Para descompressão:

1. Leitura do arquivo comprimido
2. Reconstrução da árvore de Huffman
3. Decodificação dos bits
4. Reconstrução do arquivo original

---

## Compilação

A partir da raiz do projeto:

```bash
mkdir -p bin
gcc -Wall src/compressao.c -o bin/compressao
gcc -Wall src/descompressao.c -o bin/descompressao
```

---

## Execução

Os programas leem e escrevem os arquivos `animacao.tex` e `animacao.huff` na pasta em que são executados, por isso devem ser rodados de dentro de `data/`.

### Compressão

Lê `animacao.tex` e gera `animacao.huff`:

```bash
cd data
../bin/compressao
```

### Descompressão

Lê `animacao.huff` e reconstrói `animacao.tex`:

```bash
cd data
../bin/descompressao
```

### Resultado com o arquivo de exemplo

| Arquivo | Tamanho |
| --- | --- |
| `animacao.tex` (original) | 24.472 bytes |
| `animacao.huff` (comprimido) | 11.370 bytes |

Redução de cerca de 53,5%. A descompressão reconstrói o arquivo idêntico ao original.

---

## Complexidade

| Etapa                           | Complexidade |
| ------------------------------- | ------------ |
| Construção da árvore de Huffman | O(n log n)   |
| Codificação dos símbolos        | O(n)         |
| Compressão do arquivo           | O(n)         |
| Descompressão                   | O(n)         |

---

## Contexto Acadêmico

Projeto desenvolvido como parte da disciplina de **Estruturas de Dados**, com o objetivo de aplicar conceitos de:

* Árvores binárias
* Filas de prioridade
* Algoritmos de compressão
* Manipulação de arquivos em C

---

## Autor

**Andeson da Cruz de Almeida**

Estudante de Bacharelado em Tecnologia da Informação
Interesses em Engenharia de Software, Backend e Estruturas de Dados.
