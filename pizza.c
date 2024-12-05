#include "pizza.h"
#include "ingrediente.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void adicionarPizza() {
    pizza piz;
    int ultimoID;
    int qtdIngredientes;

    FILE *arquivoPizza = fopen("pizzas.txt", "ab"); //Abre o arquivo pizzas.txt, caso não tenha, crie um.
    if (arquivoPizza == NULL) {
        printf("Erro ao abrir o arquivo pizzas.txt.\n");
        return;
    }

    FILE *idFile = fopen("pizzas_id.txt", "r+"); //Abre o arquivo pizzas_id.txt, caso não tenha, ela cria um, preenche o primeiro elemento com zero
    if (idFile == NULL) {
        idFile = fopen("pizzas_id.txt", "w+");
        if (idFile == NULL) {
            printf("Erro: nao foi possível criar o arquivo de IDs.\n");
            fclose(arquivoPizza);
            return;
        }
        fprintf(idFile, "0");
        fflush(idFile);
        rewind(idFile);
    }

    if (fscanf(idFile, "%d", &ultimoID) != 1) { //Sobrescreve o arquivo seja incialzado com zero
        ultimoID = 0;
    }

    int novoID = ultimoID + 1; //Incrementa o ID
    
    rewind(idFile); // Move o cursor para o início do arquivo
    fprintf(idFile, "%d", novoID); // Grava o novo ID
    fflush(idFile); 
    fclose(idFile); //Fecha o arquivo

    piz.id = novoID; //Atribui o ID

    //Pede os dados
    getchar(); 
    printf("Digite o nome da pizza: ");
    fgets(piz.nome, sizeof(piz.nome), stdin);
    piz.nome[strcspn(piz.nome, "\n")] = '\0';

    do {
    printf("Digite o tamanho da pizza (P, M, G): ");
    scanf(" %c", &piz.tamanho);

    if (piz.tamanho != 'P' && piz.tamanho != 'M' && piz.tamanho != 'G') {
        printf("Erro: tamanho invalido! Digite 'P', 'M' ou 'G'.\n");
    }
    } while (piz.tamanho != 'P' && piz.tamanho != 'M' && piz.tamanho != 'G');

    printf("Digite o preco base da pizza: ");
    scanf("%f", &piz.preco);

    printf("Quantos ingredientes padrao a pizza tera? (Max: 10): ");
    scanf("%d", &qtdIngredientes);

    if (qtdIngredientes > 10) {
        printf("Erro: O numero maximo de ingredientes é 10.\n");
        fclose(arquivoPizza);
        return;
    }

    getchar(); // Limpa o buffer
    for (int i = 0; i < qtdIngredientes; i++) {
        printf("Digite o nome do ingrediente %d: ", i + 1);
        fgets(piz.ingredientes[i].nome, sizeof(piz.ingredientes[i].nome), stdin);
        piz.ingredientes[i].nome[strcspn(piz.ingredientes[i].nome, "\n")] = '\0';
    }

    for (int i = qtdIngredientes; i < 10; i++) {
        strcpy(piz.ingredientes[i].nome, ""); // Zera os ingredientes não usados
    }

    fwrite(&piz, sizeof(pizza), 1, arquivoPizza); //Grava os dados no arquivo
    fclose(arquivoPizza); //Fecha o arquivo

    printf("\nPizza cadastrada com sucesso!\n");
    getchar();
}


void visualizarPizza() {
    pizza piz;
    FILE *arquivoPizza = fopen("pizzas.txt", "rb"); //Abre os arquivos em modo leitura 

    if (arquivoPizza == NULL) { // Caso não encontre pizzas cadastradas, exibe a mensagem e sai
        printf("Nenhuma pizza cadastrada.\n");
        return;
    }

    //Lendo o arquivo
    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        printf("ID: %d\n", piz.id);
        printf("Nome: %s\n", piz.nome);
        printf("Tamanho: %c\n", piz.tamanho);
        printf("Preco: %.2f\n", piz.preco);

        printf("Ingredientes:\n");
        for (int i = 0; i < 10; i++) {
            if (strlen(piz.ingredientes[i].nome) > 0) {
                printf(" - %s\n", piz.ingredientes[i].nome);
            }
        }
        printf("\n");
    }

    fclose(arquivoPizza); //Fecha os arquivos
    printf("Pressione Enter para continuar...\n");
    getchar();
    getchar();
}

void editarPizza() {
    pizza piz;
    int idPizza;
    int found = 0;

    FILE *arquivoPizza = fopen("pizzas.txt", "r+b");

    if (arquivoPizza == NULL) {
        printf("Erro ao abrir o arquivo de pizzas.\n");
        return;
    }

    printf("Digite o ID da pizza a ser editada: "); //Pergunta o id para busca da pizza no arquivo
    scanf("%d", &idPizza);


    //Pedindo os novos dados
    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (piz.id == idPizza) {
            found = 1;
            printf("Pizza com ID %d encontrada! Vamos editar os dados.\n", idPizza);

            getchar(); // Limpa o buffer
            printf("Digite o novo nome da pizza: ");
            fgets(piz.nome, sizeof(piz.nome), stdin);
            piz.nome[strcspn(piz.nome, "\n")] = '\0';

            printf("Digite o novo tamanho da pizza (P, M, G): ");
            scanf(" %c", &piz.tamanho);

            printf("Digite o novo preco base da pizza: ");
            scanf("%f", &piz.preco);

            printf("Quantos ingredientes a pizza tera? (Max: 10): ");
            int qtdIngredientes;
            scanf("%d", &qtdIngredientes);

            if (qtdIngredientes > 10) {
                printf("Erro: O número maximo de ingredientes e 10.\n");
                fclose(arquivoPizza);
                return;
            }

            getchar(); 
            for (int i = 0; i < qtdIngredientes; i++) {
                printf("Digite o nome do ingrediente %d: ", i + 1);
                fgets(piz.ingredientes[i].nome, sizeof(piz.ingredientes[i].nome), stdin);
                piz.ingredientes[i].nome[strcspn(piz.ingredientes[i].nome, "\n")] = '\0';

            }

            for (int i = qtdIngredientes; i < 10; i++) {
                strcpy(piz.ingredientes[i].nome, "");
            }

            fseek(arquivoPizza, -sizeof(pizza), SEEK_CUR); //reposiciona o ponteiro de leitura e escrita
            fwrite(&piz, sizeof(pizza), 1, arquivoPizza); //Substituindo os atributos

            printf("\nPizza com ID %d editada com sucesso!\n", idPizza);
            break;
        }
    }

    if (!found) {
        printf("Pizza com ID %d nao encontrada.\n", idPizza);
    }

    fclose(arquivoPizza); //Fecha o arquivo
    
    printf("Pressione Enter para continuar...\n");
    getchar(); 
    getchar(); 
}

void removerPizza(){
    pizza piz;
    int idPizza;
    int found = 0;

    //Abertura dos arquivos.
    FILE *arquivoPizza = fopen("pizzas.txt", "rb");
    FILE *arquivoTemp = fopen("pizzas_temp.txt", "wb");

    if (arquivoPizza == NULL || arquivoTemp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    printf("Digite o ID da pizza a ser excluida: "); //Pergunta o ID da pizza a ser excluido 
    scanf("%d", &idPizza);

    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (piz.id != idPizza) {
            fwrite(&piz, sizeof(pizza), 1, arquivoTemp); // Copia a pizza para o arquivo temporário
        } else {
            found = 1; // Encontrou a pizza a ser deletada
        }
    }

    fclose(arquivoPizza);
    fclose(arquivoTemp);

    if (found) {
        // Substitui o arquivo original pelo arquivo temporário
        remove("pizzas.txt");
        rename("pizzas_temp.txt", "pizzas.txt");
        printf("\nPizza com nome '%d' deletada com sucesso!\n", idPizza);
    } else {
        printf("Pizza com ID '%d' nao encontrada.\n", idPizza);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); 
    getchar();  
}

void venderPizza() {
    pizza piz;
    ingrediente ing;
    int idPizza, found = 0;
    float precoFinal = 0.0;
    int qtdIngredientesExtras;
    char nomeIngrediente[255];

    //Abre os arquivos
    FILE *arquivoPizza = fopen("pizzas.txt", "rb");
    FILE *arquivoIngredientes = fopen("ingredientes.txt", "rb");

    if (arquivoPizza == NULL || arquivoIngredientes == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivoPizza) fclose(arquivoPizza);
        if (arquivoIngredientes) fclose(arquivoIngredientes);
        return;
    }

    printf("Digite o ID da pizza que deseja vender: ");
    scanf("%d", &idPizza);

    // Busca a pizza selecionada pelo ID
    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (piz.id == idPizza) {
            found = 1;
            printf("Pizza com ID %d encontrada!\n", idPizza);
            printf("Nome: %s\n", piz.nome);
            printf("Tamanho: %c\n", piz.tamanho);

            printf("Ingredientes padrao:\n");
            for (int i = 0; i < 10; i++) {
                if (strlen(piz.ingredientes[i].nome) > 0) {
                    printf(" - %s\n", piz.ingredientes[i].nome);
                }
            }

            precoFinal = piz.preco; // Inicializa o preço final com o preço base
            break;
        }
    }

    if (!found) {
        printf("Pizza com ID %d nao encontrada.\n", idPizza);
        fclose(arquivoPizza);
        fclose(arquivoIngredientes);
        getchar();
        getchar();
        return;
    }

    // Permitir adicionar ingredientes extras
    printf("\nDeseja adicionar ingredientes extras? (Maximo: 10)\n");
    printf("Digite a quantidade de ingredientes extras: ");
    scanf("%d", &qtdIngredientesExtras);

    if (qtdIngredientesExtras > 10) {
        printf("Erro: Numero maximo de ingredientes extras e 10.\n");
        fclose(arquivoPizza);
        fclose(arquivoIngredientes);
        return;
    }

    getchar(); 
    for (int i = 0; i < qtdIngredientesExtras; i++) {
        printf("Digite o nome do ingrediente extra %d: ", i + 1);
        fgets(nomeIngrediente, sizeof(nomeIngrediente), stdin);
        nomeIngrediente[strcspn(nomeIngrediente, "\n")] = '\0'; 

        rewind(arquivoIngredientes); // Reinicia o cursor no arquivo de ingredientes
        int ingredienteEncontrado = 0;

        // Busca o ingrediente extra no arquivo ingredientes.txt
        while (fread(&ing, sizeof(ingrediente), 1, arquivoIngredientes) == 1) {
            if (strcmp(ing.nome, nomeIngrediente) == 0) {
                printf("Ingrediente '%s' adicionado. Preco: R$ %.2f\n", ing.nome, ing.preco);
                precoFinal += ing.preco; // Adiciona o preço do ingrediente extra ao preço final
                ingredienteEncontrado = 1;
                break;
            }
        }

        if (!ingredienteEncontrado) {
            printf("Ingrediente '%s' não encontrado. Tente novamente.\n", nomeIngrediente);
            i--; // Permite tentar novamente
        }
    }

    printf("\nPreco final da pizza (incluindo extras): R$ %.2f\n", precoFinal);

    printf("\nVenda registrada com sucesso!\n");

    fclose(arquivoPizza);
    fclose(arquivoIngredientes);

    printf("Pressione Enter para continuar...\n");
    getchar();
}

