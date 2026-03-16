# Huffman Compressão

Implementação do algoritmo de compressão de Huffman desenvolvida em linguagem C como projeto da disciplina de **Estruturas de Dados**.
O sistema permite **compressão e descompressão de arquivos utilizando codificação baseada em frequência de caracteres**, reduzindo o tamanho do arquivo original.

---

## 📖 Sobre o Projeto

A compressão de Huffman é um algoritmo clássico de compressão sem perdas que utiliza uma **árvore binária ponderada** para gerar códigos binários de tamanho variável para cada símbolo, de acordo com sua frequência de ocorrência.

Símbolos mais frequentes recebem códigos menores, enquanto símbolos menos frequentes recebem códigos maiores, resultando em uma redução do tamanho total do arquivo.

Este projeto implementa:

* Construção da **árvore de Huffman**
* Geração de **códigos binários prefix-free**
* Compressão de arquivos
* Descompressão do arquivo comprimido

---

## ⚙️ Tecnologias Utilizadas

* **C** – Linguagem principal do projeto
* **GCC** – Compilador utilizado
* **Linux / WSL** – Ambiente de desenvolvimento
* **Git** – Controle de versão

---

## 📂 Estrutura do Projeto

```
Huffman
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
|__ bin
|   |__ compressao
|   |__ descompressao
|
├── README.md
└── .gitignore
```

**src/**
Contém os arquivos de código fonte responsáveis pela implementação da compressão e descompressão.

**data/**
Contém arquivos utilizados para teste do algoritmo.

---

## 🧠 Funcionamento do Algoritmo

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

## 🚀 Compilação

Para compilar o programa de compressão:

```
gcc compressao.c -o compressao
```

Para compilar o programa de descompressão:

```
gcc descompressao.c -o descompressao
```

---

## ▶️ Execução

### Compressão

```
./compressao
```

### Descompressão

```
./descompressao
```

---

## 📊 Complexidade

| Etapa                           | Complexidade |
| ------------------------------- | ------------ |
| Construção da árvore de Huffman | O(n log n)   |
| Codificação dos símbolos        | O(n)         |
| Compressão do arquivo           | O(n)         |
| Descompressão                   | O(n)         |

---

## 🎓 Contexto Acadêmico

Projeto desenvolvido como parte da disciplina de **Estruturas de Dados**, com o objetivo de aplicar conceitos de:

* Árvores binárias
* Filas de prioridade
* Algoritmos de compressão
* Manipulação de arquivos em C

---

## 👨‍💻 Autor

**Anderson Almeida**

Estudante de Bacharelado em Tecnologia da Informação
Interesses em Engenharia de Software, Backend e Estruturas de Dados.
