#include <stdio.h>

// Definimos uma estrutura (struct) básica
struct Aluno {
    int matricula; // Usamos número para identificar o aluno facilmente
    float notas[4]; // 4 bimestres
    float media;
    char conceito;
};

// Função para calcular a média e definir o conceito (A, B, C, D)
void calcularConceito(struct Aluno *a) {
    a->media = (a->notas[0] + a->notas[1] + a->notas[2] + a->notas[3]) / 4.0;

    // Regra de conceitos
    if (a->media >= 9.0 && a->media <= 10.0) {
        a->conceito = 'A';
    } else if (a->media >= 6.0 && a->media < 9.0) {
        a->conceito = 'B';
    } else if (a->media >= 3.0 && a->media < 6.0) {
        a->conceito = 'C';
    } else {
        a->conceito = 'D'; // de 0 até 3
    }
}

int main() {
    struct Aluno alunos[100]; // Vetor de alunos
    int totalAlunos = 0;
    int opcao;

    do {
        printf("\n--- SISTEMA DE NOTAS ESCOLARES (CRUD) ---\n");
        printf("1. Cadastrar Aluno\n");
        printf("2. Listar Alunos\n");
        printf("3. Atualizar Notas de um Aluno\n");
        printf("4. Excluir Aluno\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) { // CREATE (Criar)
            if (totalAlunos < 100) {
                printf("\n--- Cadastro de Aluno ---\n");
                printf("Digite o numero de matricula do aluno (ex: 101): ");
                scanf("%d", &alunos[totalAlunos].matricula);

                for (int i = 0; i < 4; i++) {
                    printf("Digite a nota do %dº bimestre (0 a 10): ", i + 1);
                    scanf("%f", &alunos[totalAlunos].notas[i]);
                }

                calcularConceito(&alunos[totalAlunos]);
                totalAlunos++;
                printf("\nAluno cadastrado com sucesso!\n");
            } else {
                printf("Limite de alunos atingido!\n");
            }

        } else if (opcao == 2) { // READ (Ler / Listar)
            if (totalAlunos == 0) {
                printf("\nNenhum aluno cadastrado.\n");
            } else {
                printf("\n--- LISTA DE ALUNOS ---\n");
                for (int i = 0; i < totalAlunos; i++) {
                    printf("Matricula: %d\n", alunos[i].matricula);
                    printf("Notas: %.1f | %.1f | %.1f | %.1f\n", alunos[i].notas[0], alunos[i].notas[1], alunos[i].notas[2], alunos[i].notas[3]);
                    printf("Media: %.2f | Conceito: %c\n", alunos[i].media, alunos[i].conceito);
                    printf("----------------------------------------\n");
                }
            }

        } else if (opcao == 3) { // UPDATE (Atualizar notas)
            int matBusca, encontrado = 0;
            printf("\nDigite a matricula do aluno que deseja atualizar: ");
            scanf("%d", &matBusca);

            for (int i = 0; i < totalAlunos; i++) {
                if (alunos[i].matricula == matBusca) {
                    printf("Atualizando notas para a matricula: %d\n", alunos[i].matricula);
                    for (int j = 0; j < 4; j++) {
                        printf("Nova nota do %dº bimestre: ", j + 1);
                        scanf("%f", &alunos[i].notas[j]);
                    }
                    calcularConceito(&alunos[i]);
                    printf("Notas atualizadas e conceito recalculado!\n");
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("Aluno nao encontrado.\n");
            }

        } else if (opcao == 4) { // DELETE (Excluir)
            int matBusca, encontrado = 0;
            printf("\nDigite a matricula do aluno que deseja excluir: ");
            scanf("%d", &matBusca);

            for (int i = 0; i < totalAlunos; i++) {
                if (alunos[i].matricula == matBusca) {
                    // Move os elementos seguintes para cobrir o espaço
                    for (int j = i; j < totalAlunos - 1; j++) {
                        alunos[j] = alunos[j + 1];
                    }
                    totalAlunos--;
                    printf("Aluno excluido com sucesso!\n");
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                printf("Aluno nao encontrado.\n");
            }

        } else if (opcao == 0) {
            printf("\nEncerrando o programa...\n");
        } else {
            printf("\nOpcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}