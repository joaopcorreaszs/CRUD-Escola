/* ============================================================
   SISTEMA DE NOTAS ESCOLARES - CRUD com FILA (FIFO)
   ------------------------------------------------------------
   Estruturas de dados possiveis: lista, fila, pilha, arvore, grafo.

   ESCOLHA: FILA ENCADEADA (FIFO - First In, First Out)
   Por que a fila e a melhor opcao aqui:
   - Alunos sao cadastrados e atendidos por ordem de chegada
     (primeiro aluno cadastrado e o primeiro da fila).
   - A fila encadeada nao tem limite fixo de 100 alunos:
     a memoria cresce conforme a necessidade (malloc).
   - As operacoes do CRUD ficam simples: inserir no fim (enqueue),
     remover do inicio (dequeue), buscar percorrendo a fila.

   Outras estruturas e por que NAO foram escolhidas:
   - Lista: tambem serviria, e ate mais flexivel, mas a fila deixa
     a regra "quem chegou primeiro e atendido primeiro" explicita.
   - Pilha (LIFO): so aceitaria o ultimo aluno cadastrado, o que
     nao combina com "gerenciar notas de varios alunos".
   - Arvore: otima para busca rapida por matricula (ordenada),
     mas e mais complexa do que o problema pede. Dica para depois:
     se a escola tiver milhares de alunos, use arvore binaria.
   - Grafo: serve para relacoes (ex.: aluno X e amigo de Y), nao
     para um cadastro de notas.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BIMESTRES 4

/* ---------- STRUCT DO ALUNO (dados do problema) ---------- */
struct Aluno {
    int matricula;
    float notas[NUM_BIMESTRES];
    float media;
    char conceito;
};

/* ---------- ESTRUTURA ESCOLHIDA: FILA ENCADEADA ---------- */
/* E o "no" da fila: guarda o aluno e o ponteiro para o proximo. */
struct NoFila {
    struct Aluno dado;
    struct NoFila *prox;
};

struct Fila {
    struct NoFila *inicio; /* primeiro aluno que entrou  */
    struct NoFila *fim;    /* ultimo aluno que entrou    */
    int tamanho;
};

/* Cria a fila vazia. */
void inicializarFila(struct Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

/* ENQUEUE: insere o aluno no fim da fila (Create).
   Devolve 1 se deu certo e 0 se faltou memoria. */
int enfileirar(struct Fila *f, struct Aluno a) {
    struct NoFila *novo = (struct NoFila *) malloc(sizeof(struct NoFila));
    if (novo == NULL) {
        return 0;
    }
    novo->dado = a;
    novo->prox = NULL;

    if (f->fim == NULL) {          /* fila vazia */
        f->inicio = novo;
        f->fim = novo;
    } else {                       /* coloca depois do ultimo */
        f->fim->prox = novo;
        f->fim = novo;
    }
    f->tamanho++;
    return 1;
}

/* DEQUEUE: remove o primeiro aluno da fila e copia os dados
   para *saida (Delete). Devolve 1 se removeu, 0 se a fila estava vazia. */
int desenfileirar(struct Fila *f, struct Aluno *saida) {
    struct NoFila *alvo = f->inicio;
    if (alvo == NULL) {
        return 0;
    }
    if (saida != NULL) {
        *saida = alvo->dado;
    }
    f->inicio = alvo->prox;
    if (f->inicio == NULL) {   /* a fila ficou vazia */
        f->fim = NULL;
    }
    free(alvo);
    f->tamanho--;
    return 1;
}

/* Libera todos os nos da fila (evita vazamento de memoria). */
void destruirFila(struct Fila *f) {
    struct Aluno descartado;
    while (desenfileirar(f, &descartado)) {
        /* nada a fazer: desenfileirar ja libera o no */
    }
}

/* ---------------- AJUDANTES DE APARENCIA ---------------- */

/* Largura interna das caixas. Se mudar aqui, o menu tambem se ajusta. */
#define LARGURA 66

/* Desenha o texto centralizado dentro de uma linha '| ... |'.
   A conta usa LARGURA - 2 porque os dois '|' ocupam 2 colunas. */
void linhaCentrada(const char *texto) {
    int espacoInterno = LARGURA - 2;
    int tam = (int) strlen(texto);
    int espacos = (espacoInterno - tam) / 2;
    if (espacos < 0) {
        espacos = 0;
    }
    int restantes = espacoInterno - espacos - tam;
    if (restantes < 0) {
        restantes = 0;
    }
    printf("|%*s%s%*s|\n", espacos, "", texto, restantes, "");
}

/* Linha horizontal: +----+ */
void linha(char c) {
    putchar('+');
    for (int i = 0; i < LARGURA; i++) {
        putchar(c);
    }
    printf("+\n");
}

/* Tela de titulo de uma secao, dentro de uma moldura. */
void titulo(const char *texto) {
    printf("\n");
    linha('=');
    linhaCentrada(texto);
    linha('=');
}

/* Mensagens de sucesso e de erro, com marcador visual. */
void avisoOk(const char *texto) {
    printf("  [OK] %s\n", texto);
}

void avisoErro(const char *texto) {
    printf("  [!] %s\n", texto);
}

/* READ: percorre a fila com um ponteiro auxiliar, do inicio ate o fim. */
void listarAlunos(struct Fila *f) {
    if (f->inicio == NULL) {
        titulo("LISTA DE ALUNOS");
        printf("  Nenhum aluno cadastrado ainda.\n");
        return;
    }

    titulo("LISTA DE ALUNOS (ordem da fila)");

    /* Cabecalho da tabela. As colunas somam LARGURA-2 = 64 colunas:
       3 + 3 + 10 + 3 + 20 + 3 + 7 + 3 + 5 + 3+1(bordas) = 64 */
    printf("| %-3s | %-10s | %-20s | %-7s | %-5s|\n",
           "#", "MATRICULA", "NOTAS (1o a 4o)", "MEDIA", "CONC");
    linha('-');

    int posicao = 1;
    int totalA = 0, totalB = 0, totalC = 0, totalD = 0;
    struct NoFila *atual = f->inicio;

    while (atual != NULL) {
        struct Aluno *a = &atual->dado;

        /* Monta as 4 notas em um texto so, para caber na coluna. */
        char notas[40];
        snprintf(notas, sizeof(notas), "%.1f  %.1f  %.1f  %.1f",
                 a->notas[0], a->notas[1], a->notas[2], a->notas[3]);

        printf("| %-3d | %-10d | %-20s | %-7.2f | %-5c|\n",
               posicao, a->matricula, notas, a->media, a->conceito);

        switch (a->conceito) {
            case 'A': totalA++; break;
            case 'B': totalB++; break;
            case 'C': totalC++; break;
            default:  totalD++; break;
        }

        atual = atual->prox;
        posicao++;
    }

    linha('-');
    printf("  Total de alunos: %d\n", f->tamanho);
    printf("  Conceitos: A(%d)  B(%d)  C(%d)  D(%d)\n",
           totalA, totalB, totalC, totalD);
}

/* READ com parametro: busca a matricula e devolve o ponteiro para o no,
   ou NULL se nao encontrar. Usado pelo Update e pelo Delete. */
struct NoFila *buscarAluno(struct Fila *f, int matricula) {
    struct NoFila *atual = f->inicio;
    while (atual != NULL) {
        if (atual->dado.matricula == matricula) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/* Calcula a media e o conceito A, B, C ou D. */
void calcularConceito(struct Aluno *a) {
    float soma = 0.0f;
    for (int i = 0; i < NUM_BIMESTRES; i++) {
        soma += a->notas[i];
    }
    a->media = soma / NUM_BIMESTRES;

    if (a->media >= 9.0f) {
        a->conceito = 'A';      /* 9 a 10 */
    } else if (a->media >= 6.0f) {
        a->conceito = 'B';      /* 6 a 9  */
    } else if (a->media >= 3.0f) {
        a->conceito = 'C';      /* 3 a 6  */
    } else {
        a->conceito = 'D';      /* 0 a 3  */
    }
}

/* Le uma nota valida (0 a 10). Se o usuario digitar qualquer outra
   coisa que nao seja numero, a mensagem de erro aparece na hora. */
float lerNota(const char *rotulo) {
    float valor;
    while (1) {
        printf("    %s", rotulo);
        if (scanf("%f", &valor) != 1) {
            avisoErro("Entrada invalida! Digite um numero.");
            scanf("%*s"); /* descarta o texto digitado */
            continue;
        }
        if (valor < 0.0f || valor > 10.0f) {
            avisoErro("Nota fora da faixa! Use valores de 0 a 10.");
            continue;
        }
        return valor;
    }
}

/* Le um numero inteiro valido (menu, matricula). */
int lerInteiro(const char *rotulo) {
    int valor;
    while (1) {
        printf("%s", rotulo);
        if (scanf("%d", &valor) == 1) {
            return valor;
        }
        avisoErro("Entrada invalida! Digite um numero inteiro.");
        scanf("%*s");
    }
}

/* ======================== CRUD ======================== */

/* CREATE */
void cadastrarAluno(struct Fila *f) {
    struct Aluno novo;
    titulo("CADASTRO DE ALUNO");
    novo.matricula = lerInteiro("    Matricula do aluno (ex: 101): ");

    if (buscarAluno(f, novo.matricula) != NULL) {
        avisoErro("Ja existe um aluno com essa matricula!");
        return;
    }

    printf("    Digite as 4 notas (0 a 10):\n");
    for (int i = 0; i < NUM_BIMESTRES; i++) {
        char rotulo[64];
        snprintf(rotulo, sizeof(rotulo), "%dº bimestre: ", i + 1);
        novo.notas[i] = lerNota(rotulo);
    }

    calcularConceito(&novo);

    if (enfileirar(f, novo)) {
        char msg[100];
        snprintf(msg, sizeof(msg),
                 "Aluno %d cadastrado!  Media %.2f  |  Conceito %c",
                 novo.matricula, novo.media, novo.conceito);
        avisoOk(msg);
    } else {
        avisoErro("Memoria insuficiente para cadastrar o aluno.");
    }
}

/* READ */
void listar(struct Fila *f) {
    listarAlunos(f);
}

/* UPDATE */
void atualizarNotas(struct Fila *f) {
    titulo("ATUALIZAR NOTAS");
    int matricula = lerInteiro("    Matricula do aluno: ");

    struct NoFila *no = buscarAluno(f, matricula);
    if (no == NULL) {
        avisoErro("Aluno nao encontrado.");
        return;
    }

    printf("    Aluno encontrado (matricula %d). Informe as novas notas:\n",
           no->dado.matricula);
    for (int i = 0; i < NUM_BIMESTRES; i++) {
        char rotulo[64];
        snprintf(rotulo, sizeof(rotulo), "Nova nota do %dº bimestre (0 a 10): ", i + 1);
        no->dado.notas[i] = lerNota(rotulo);
    }
    calcularConceito(&no->dado);

    char msg[100];
    snprintf(msg, sizeof(msg), "Notas atualizadas!  Media %.2f  |  Conceito %c",
             no->dado.media, no->dado.conceito);
    avisoOk(msg);
}

/* DELETE
   Remove pelo meio: percorre a fila guardando o no anterior e
   religa os ponteiros, sem perder o resto da fila. */
void excluirAluno(struct Fila *f) {
    titulo("EXCLUIR ALUNO");
    if (f->inicio == NULL) {
        avisoErro("Nenhum aluno cadastrado.");
        return;
    }

    int matricula = lerInteiro("    Matricula do aluno a excluir: ");

    struct NoFila *anterior = NULL;
    struct NoFila *atual = f->inicio;

    while (atual != NULL && atual->dado.matricula != matricula) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        avisoErro("Aluno nao encontrado.");
        return;
    }

    if (anterior == NULL) {          /* era o primeiro da fila */
        desenfileirar(f, NULL);
    } else {                          /* estava no meio ou no fim */
        anterior->prox = atual->prox;
        if (f->fim == atual) {        /* era o ultimo: corrige o fim */
            f->fim = anterior;
        }
        free(atual);
        f->tamanho--;
    }
    avisoOk("Aluno excluido com sucesso!");
}

/* Operacao extra do TAD fila: atender (remover) o primeiro da fila. */
void atenderPrimeiro(struct Fila *f) {
    struct Aluno atendido;
    titulo("ATENDER PROXIMO DA FILA");
    if (desenfileirar(f, &atendido)) {
        char msg[120];
        snprintf(msg, sizeof(msg),
                 "Atendido: matricula %d  |  Media %.2f  |  Conceito %c",
                 atendido.matricula, atendido.media, atendido.conceito);
        avisoOk(msg);
        printf("  Alunos restantes na fila: %d\n", f->tamanho);
    } else {
        avisoErro("A fila esta vazia.");
    }
}

/* Desenha o menu principal. */
void mostrarMenu(struct Fila *f) {
    char rodape[80];
    snprintf(rodape, sizeof(rodape), "Alunos na fila: %d", f->tamanho);

    printf("\n");
    linha('=');
    linhaCentrada("SISTEMA DE NOTAS ESCOLARES");
    linhaCentrada("CRUD com Fila (FIFO)");
    linha('=');
    printf("|  %-62s|\n", "1 - Cadastrar aluno       (entra no fim da fila)");
    printf("|  %-62s|\n", "2 - Listar alunos         (tabela com medias)");
    printf("|  %-62s|\n", "3 - Atualizar notas       (por matricula)");
    printf("|  %-62s|\n", "4 - Excluir aluno         (por matricula)");
    printf("|  %-62s|\n", "5 - Atender proximo       (sai o primeiro da fila)");
    printf("|  %-62s|\n", "0 - Sair");
    linha('-');
    linhaCentrada(rodape);
    linha('=');
}

/* Menu principal. */
int main(void) {
    struct Fila fila;
    inicializarFila(&fila);

    int opcao;
    do {
        mostrarMenu(&fila);
        opcao = lerInteiro("  Opcao: ");

        switch (opcao) {
            case 1: cadastrarAluno(&fila);  break;
            case 2: listar(&fila);          break;
            case 3: atualizarNotas(&fila);  break;
            case 4: excluirAluno(&fila);    break;
            case 5: atenderPrimeiro(&fila); break;
            case 0:
                printf("\n");
                linha('=');
                linhaCentrada("Ate logo! Programa encerrado.");
                linha('=');
                break;
            default:
                avisoErro("Opcao invalida! Digite um numero do menu.");
        }
    } while (opcao != 0);

    destruirFila(&fila);
    return 0;
}
