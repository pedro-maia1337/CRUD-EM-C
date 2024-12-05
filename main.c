#include "pizza.h"
#include "ingrediente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

void cabecalho();

int main(void){
  int op;

    do{
        cabecalho();
        printf("1 - Adicionar Pizza\n");
        printf("2 - Visualizar Pizza\n");
        printf("3 - Editar Pizza\n");
        printf("4 - Remover Pizza\n");

        printf("5 - Adicionar Ingrediente\n");
        printf("6 - Visualizar Ingrediente\n");
        printf("7 - Editar Ingrediente\n");
        printf("8 - Remover Ingrediente\n");

        printf("9 - Vender Pizza\n");

        printf("0 - Sair\n");

        printf("\n");

        printf("Escolha uma opcao: ");


        scanf("%d", &op);

        switch (op) {
            case 1:
                adicionarPizza();
                break;

            case 2:
                visualizarPizza();
                break;

            case 3:
                editarPizza();
                break;

            case 4:
                removerPizza();
                break;

            case 5:
                adicionarIngrediente();
                break;

            case 6:
                visualizarIngrediente();
                break;

            case 7:
                editarIngrediente();
                break;

            case 8:
                removerIngrediente();
                break;

            case 9:
                venderPizza();
                break;

            case 0:
                printf("Pressione Enter para sair.....");
                getch();
                break;
            
            default:
                printf("Opcao invalida!!!");
                getch();
            }

    } while (op != 0); 

    return 0;
}

void cabecalho() {
    system("cls");
    printf("-----------------------------------\n");
    printf("Gerencimento de pizzaria\n");
    printf("-----------------------------------\n");
    printf("\n");
}



