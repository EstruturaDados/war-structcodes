#include <stdio.h>
#include <string.h>

// Definindo a struct Territorio
typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

int main() {
    Territorio territorios[5]; // vetor estático de 5 elementos

    printf("=== Cadastro dos Territorios ===\n\n");

    // Cadastrar dados dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, 50, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // remove \n

        printf("Cor do Exercito: ");
        fgets(territorios[i].corExercito, 20, stdin);
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = '\0'; // remove \n

        printf("Numero de Tropas: ");
        scanf("%d", &territorios[i].numTropas);
        getchar(); // limpa o \n do buffer

        printf("\n");
    }

    // Exibir o estado atual do mapa
    printf("=== Estado Atual do Mapa ===\n\n");
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do Exercito: %s\n", territorios[i].corExercito);
        printf("Numero de Tropas: %d\n", territorios[i].numTropas);
        printf("---------------------------\n");
    }

    return 0;
}
