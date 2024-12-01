#include "pizza.h"
#include "ingrediente.h"
#include <stdlib.h>
#include <string.h>  
#include <stdio.h>

void adicionarPizza(){
    pizza piz;
    ingrediente ing;
    int qtdIngredientes, i;
    char nomeSelecionado[255];
    int ingredienteEncontrado = 0;
    int ultimoID;

    FILE *arquivoPizza = fopen("pizzas.txt", "ab");
    FILE *arquivoIngredientes = fopen("ingredientes.txt", "rb");

    if (arquivoPizza == NULL || arquivoIngredientes == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivoPizza) fclose(arquivoPizza);
        if (arquivoIngredientes) fclose(arquivoIngredientes);
        return;
    }

    FILE *idFile = fopen("pizzas_id.txt", "r+");
    if (idFile == NULL) {
        idFile = fopen("pizzas_id.txt", "w+");
        if (idFile == NULL) {
            printf("Erro: nao foi possivel criar o arquivo de IDs.\n");
            exit(1);
        }
        fprintf(idFile, "0"); // Inicializa o ID com 0
        fflush(idFile);
        rewind(idFile);
    }

     // Lê o último ID
    if (fscanf(idFile, "%d", &ultimoID) != 1) {
        ultimoID = 0; // Define como 0 se a leitura falhar
    }

    // Incrementa o ID
    int novoID = ultimoID + 1;

    // Atualiza o ID no arquivo
    rewind(idFile); // Move o cursor para o início do arquivo
    fprintf(idFile, "%d", novoID); // Grava o novo ID
    fflush(idFile);
    fclose(idFile);

    // Dados da pizza
    piz.id = novoID;

    getchar(); // Limpa o buffer do teclado
    printf("Digite o nome da pizza: ");
    fgets(piz.nome, sizeof(piz.nome), stdin);
    piz.nome[strcspn(piz.nome, "\n")] = '\0'; // Remove o '\n'

    printf("Digite o tamanho da pizza (P, M, G): ");
    scanf(" %c", &piz.tamanho);

    printf("Digite o preco base da pizza: ");
    scanf("%f", &piz.preco);

    // Seleção de ingredientes já cadastrados
    printf("Insira os numeros de ingredientes? (Max: %d): ", 10);
    scanf("%d", &qtdIngredientes);

    if (qtdIngredientes > 10) {
        printf("Número de ingredientes excede o limite permitido.\n");
        fclose(arquivoPizza);
        fclose(arquivoIngredientes);
        return;
    }

    piz.qtdIngredientes = qtdIngredientes; // Define a quantidade de ingredientes
    getchar(); // Limpa o buffer

    for (i = 0; i < qtdIngredientes; i++) {
        printf("Digite o nome do ingrediente %d: ", i + 1);
        fgets(nomeSelecionado, sizeof(nomeSelecionado), stdin);
        nomeSelecionado[strcspn(nomeSelecionado, "\n")] = '\0'; // Remove o '\n'

        rewind(arquivoIngredientes); // Reinicia o cursor no arquivo de ingredientes
        ingredienteEncontrado = 0;

        // Busca o ingrediente pelo nome
        while (fread(&ing, sizeof(ingrediente), 1, arquivoIngredientes) == 1) {
            if (strcmp(ing.nome, nomeSelecionado) == 0) {
                piz.ingredientes[i] = ing; // Adiciona o ingrediente à pizza
                ingredienteEncontrado = 1;
                break;
            }
        }

        if (!ingredienteEncontrado) {
            printf("Ingrediente com nome '%s' não encontrado\n", nomeSelecionado);
        }
    }

    // Salva a pizza no arquivo
    fwrite(&piz, sizeof(piz), 1, arquivoPizza);
    fclose(arquivoPizza);
    fclose(arquivoIngredientes);

    printf("\nPizza cadastrada com sucesso!\n");

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar();
}

void visualizarPizza(){
    pizza piz;
    FILE *arquivoPizza = fopen("pizzas.txt", "rb");

    if (arquivoPizza == NULL) {
        printf("Nenhuma pizza cadastrada.\n");
        return;
    }

    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        printf("ID: %d\n", piz.id);
        printf("Nome: %s\n", piz.nome);
        printf("Tamanho: %c\n", piz.tamanho);
        printf("Preco: %.2f\n", piz.preco);
     
        printf("Ingredientes:\n");
        for (int i = 0; i < piz.qtdIngredientes; i++) { // Usar piz.qtdIngredientes
            printf("Nome do ingrediente: %s, Preco do ingrediente: %.2f\n",
                   piz.ingredientes[i].nome,
                   piz.ingredientes[i].preco);
        }

        printf("\n");
    }

    fclose(arquivoPizza);

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar();
}


void editarPizza(){
    pizza piz;
    char nomePizza[100];
    int found = 0;
    FILE *arquivoPizza = fopen("pizzas.txt", "r+b");

    if (arquivoPizza == NULL) {
        printf("Erro ao abrir o arquivo de pizzas.\n");
        return;
    }

    getchar(); // Limpa o buffer do teclado
    printf("Digite o nome da pizza a ser editada: ");
    fgets(nomePizza, sizeof(nomePizza), stdin);
    nomePizza[strcspn(nomePizza, "\n")] = '\0'; // Remove o '\n'

    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (strcmp(piz.nome, nomePizza) == 0) {
            found = 1;
            printf("Pizza encontrada! Vamos editar os dados.\n");

            // Edita o nome da pizza
            printf("Digite o novo nome da pizza: ");
            fgets(piz.nome, sizeof(piz.nome), stdin);
            piz.nome[strcspn(piz.nome, "\n")] = '\0'; // Remove o '\n'

            // Edita o tamanho da pizza
            printf("Digite o novo tamanho da pizza (P, M, G): ");
            scanf(" %c", &piz.tamanho);

            // Edita o preço base da pizza
            printf("Digite o novo preço base da pizza: ");
            scanf("%f", &piz.preco);

            // Reposiciona o ponteiro de arquivo para sobrescrever os dados
            fseek(arquivoPizza, -sizeof(pizza), SEEK_CUR);
            fwrite(&piz, sizeof(pizza), 1, arquivoPizza);

            printf("\nPizza editada com sucesso!\n");
            break;
        }
    }

    if (!found) {
        printf("Pizza com nome '%s' não encontrada.\n", nomePizza);
    }

    fclose(arquivoPizza);

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}

void removerPizza(){
    pizza piz;
    char nomePizza[100];
    int found = 0;
    FILE *arquivoPizza = fopen("pizzas.txt", "rb");
    FILE *arquivoTemp = fopen("pizzas_temp.txt", "wb");

    if (arquivoPizza == NULL || arquivoTemp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    getchar(); // Limpa o buffer do teclado
    printf("Digite o nome da pizza a ser deletada: ");
    fgets(nomePizza, sizeof(nomePizza), stdin);
    nomePizza[strcspn(nomePizza, "\n")] = '\0'; // Remove o '\n'

    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (strcmp(piz.nome, nomePizza) != 0) {
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
        printf("\nPizza com nome '%s' deletada com sucesso!\n", nomePizza);
    } else {
        printf("Pizza com nome '%s' não encontrada.\n", nomePizza);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}
