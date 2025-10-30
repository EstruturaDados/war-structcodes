#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

// Função para ordenar dados decrescentemente (selection sort simples)
void sort_desc(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] > arr[max_idx]) max_idx = j;
        }
        if (max_idx != i) {
            int tmp = arr[i];
            arr[i] = arr[max_idx];
            arr[max_idx] = tmp;
        }
    }
}

// Função para simular ataque entre dois territórios
void simulate_attack(Territorio *attacker, Territorio *defender) {
    if (attacker->numTropas < 2) {
        printf("Ataque impossivel: %s tem menos de 2 tropas.\n", attacker->nome);
        return;
    }
    if (defender->numTropas <= 0) {
        printf("Defensor %s nao tem tropas.\n", defender->nome);
        return;
    }

    int attDice = attacker->numTropas - 1; // nao pode atacar com a ultima tropa
    if (attDice > 3) attDice = 3;

    int defDice = defender->numTropas;
    if (defDice > 2) defDice = 2;

    int attRolls[3] = {0,0,0};
    int defRolls[2] = {0,0};

    for (int i = 0; i < attDice; i++) attRolls[i] = (rand() % 6) + 1;
    for (int i = 0; i < defDice; i++) defRolls[i] = (rand() % 6) + 1;

    sort_desc(attRolls, attDice);
    sort_desc(defRolls, defDice);

    printf("\n--- Batalha: %s (att: %d tropas)  vs  %s (def: %d tropas) ---\n",
           attacker->nome, attacker->numTropas, defender->nome, defender->numTropas);
    printf("Atacante rolagens: ");
    for (int i = 0; i < attDice; i++) printf("%d ", attRolls[i]);
    printf("\nDefensor rolagens:  ");
    for (int i = 0; i < defDice; i++) printf("%d ", defRolls[i]);
    printf("\n");

    int comparisons = (attDice < defDice) ? attDice : defDice;
    for (int i = 0; i < comparisons; i++) {
        if (attRolls[i] >= defRolls[i]) { // empate favorece atacante
            defender->numTropas--;
            printf("Comparacao %d: atacante ganha -> defensor %s perde 1 tropa.\n", i+1, defender->nome);
        } else {
            attacker->numTropas--;
            printf("Comparacao %d: defensor ganha -> atacante %s perde 1 tropa.\n", i+1, attacker->nome);
        }
    }

    if (defender->numTropas <= 0) {
        printf("\n==> %s conquistou %s!\n", attacker->nome, defender->nome);
        attacker->numTropas--; // mover 1 tropa
        if (attacker->numTropas < 0) attacker->numTropas = 0;
        defender->numTropas = 1;
        strncpy(defender->corExercito, attacker->corExercito, sizeof(defender->corExercito)-1);
        defender->corExercito[sizeof(defender->corExercito)-1] = '\0';
    }

    printf("Estado apos batalha:\n");
    printf("  %s -> tropa(s): %d, cor: %s\n", attacker->nome, attacker->numTropas, attacker->corExercito);
    printf("  %s -> tropa(s): %d, cor: %s\n", defender->nome, defender->numTropas, defender->corExercito);
    printf("----------------------------------------------------\n");
}

int main() {
    srand((unsigned) time(NULL)); // inicializa gerador de aleatorios

    int qtd;
    printf("Quantos territorios deseja cadastrar? ");
    if (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Valor invalido.\n");
        return 1;
    }
    getchar(); // limpa \n

    Territorio *territorios = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // Cadastro dos territórios
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritorio %d:\n", i+1);
        printf("Nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do Exercito: ");
        fgets(territorios[i].corExercito, sizeof(territorios[i].corExercito), stdin);
        territorios[i].corExercito[strcspn(territorios[i].corExercito, "\n")] = '\0';

        printf("Numero de Tropas: ");
        if (scanf("%d", &territorios[i].numTropas) != 1) territorios[i].numTropas = 0;
        getchar(); // limpa \n
    }

    // Menu interativo para ataques
    while (1) {
        printf("\n=== Estado Atual dos Territorios ===\n");
        for (int i = 0; i < qtd; i++) {
            printf("%d) %s -- Tropas: %d -- Cor: %s\n", i+1, territorios[i].nome, territorios[i].numTropas, territorios[i].corExercito);
        }
        printf("-----------------------------------\n");
        printf("Escolha o TERRITORIO ATACANTE (1 a %d, ou 0 para sair): ", qtd);
        int ai;
        if (scanf("%d", &ai) != 1) { getchar(); continue; }
        if (ai == 0) break;
        printf("Escolha o TERRITORIO DEFENSOR (1 a %d): ", qtd);
        int di;
        if (scanf("%d", &di) != 1) { getchar(); continue; }
        getchar(); // limpa \n

        if (ai < 1 || ai > qtd || di < 1 || di > qtd) {
            printf("Indices invalidos.\n");
            continue;
        }
        if (ai == di) {
            printf("Nao pode atacar o mesmo territorio.\n");
            continue;
        }

        simulate_attack(&territorios[ai-1], &territorios[di-1]);
    }

    free(territorios);
    printf("Programa finalizado.\n");
    return 0;
}
