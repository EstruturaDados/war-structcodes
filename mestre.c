#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[50];
    char corExercito[20];
    int numTropas;
} Territorio;

// Função para ordenar dados decrescentemente
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

// Função que exibe o estado atual dos territórios
void mostrar_territorios(const Territorio *territorios, int qtd) {
    printf("\n=== Estado Atual dos Territorios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("%d) %s -- Tropas: %d -- Cor: %s\n", i + 1, territorios[i].nome,
               territorios[i].numTropas, territorios[i].corExercito);
    }
    printf("-----------------------------------\n");
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

    int attDice = attacker->numTropas - 1;
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
        if (attRolls[i] >= defRolls[i]) {
            defender->numTropas--;
            printf("Comparacao %d: atacante ganha -> defensor %s perde 1 tropa.\n", i + 1, defender->nome);
        } else {
            attacker->numTropas--;
            printf("Comparacao %d: defensor ganha -> atacante %s perde 1 tropa.\n", i + 1, attacker->nome);
        }
    }

    if (defender->numTropas <= 0) {
        printf("\n==> %s conquistou %s!\n", attacker->nome, defender->nome);
        attacker->numTropas--;
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

// Função que verifica o cumprimento das missões
void verificar_missoes(const Territorio *territorios, int qtd) {
    int conquistados = 0;
    int verde_existe = 0;

    for (int i = 0; i < qtd; i++) {
        if (strcmp(territorios[i].corExercito, "Verde") == 0 && territorios[i].numTropas > 0) {
            verde_existe = 1;
        }
        if (strcmp(territorios[i].corExercito, "Seu") == 0) { // supondo cor do jogador = "Seu"
            conquistados++;
        }
    }

    printf("\n=== Verificação de Missões ===\n");
    if (!verde_existe) {
        printf("Missao 1: Destruir o exercito Verde -> COMPLETA!\n");
    } else {
        printf("Missao 1: Destruir o exercito Verde -> INCOMPLETA.\n");
    }

    if (conquistados >= 3) {
        printf("Missao 2: Conquistar 3 territorios -> COMPLETA!\n");
    } else {
        printf("Missao 2: Conquistar 3 territorios -> INCOMPLETA (%d/3).\n", conquistados);
    }
    printf("-----------------------------------\n");
}

// Função que inicializa os territórios (automática)
void inicializar_territorios(Territorio *territorios, int qtd) {
    const char *nomes[] = {"Alpha", "Beta", "Gamma", "Delta", "Epsilon"};
    const char *cores[] = {"Vermelho", "Azul", "Verde", "Amarelo", "Roxo"};
    for (int i = 0; i < qtd; i++) {
        strncpy(territorios[i].nome, nomes[i % 5], sizeof(territorios[i].nome)-1);
        strncpy(territorios[i].corExercito, cores[i % 5], sizeof(territorios[i].corExercito)-1);
        territorios[i].numTropas = (rand() % 5) + 1; // 1 a 5 tropas
    }
}

// Função principal do menu
void menu_principal(Territorio *territorios, int qtd) {
    while (1) {
        printf("\n=== Menu Principal ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missões\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        int opc;
        if (scanf("%d", &opc) != 1) { getchar(); continue; }
        getchar(); // limpa \n

        if (opc == 0) break;

        switch(opc) {
            case 1: {
                mostrar_territorios(territorios, qtd);
                printf("Escolha o TERRITORIO ATACANTE (1 a %d): ", qtd);
                int ai; if(scanf("%d", &ai)!=1){getchar(); continue;}
                printf("Escolha o TERRITORIO DEFENSOR (1 a %d): ", qtd);
                int di; if(scanf("%d", &di)!=1){getchar(); continue;}
                getchar();
                if (ai < 1 || ai > qtd || di < 1 || di > qtd || ai == di) {
                    printf("Indices invalidos.\n"); break;
                }
                simulate_attack(&territorios[ai-1], &territorios[di-1]);
                break;
            }
            case 2:
                verificar_missoes(territorios, qtd);
                break;
            default:
                printf("Opcao invalida.\n");
        }
    }
}

int main() {
    srand((unsigned) time(NULL));

    int qtd = 5; // quantidade de territórios padrão
    Territorio *territorios = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (!territorios) { printf("Erro ao alocar memoria.\n"); return 1; }

    inicializar_territorios(territorios, qtd);
    menu_principal(territorios, qtd);

    free(territorios);
    printf("Programa finalizado.\n");
    return 0;
}
