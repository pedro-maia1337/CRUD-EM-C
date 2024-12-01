#include "ingrediente.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void adicionarIngrediente() {
    ingrediente ing;
    int ultimoID = 0;

    // Abre o arquivo de ingredientes para escrita
    FILE *arquivo = fopen("ingredientes.txt", "ab");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo de ingredientes.\n");
        exit(1);
    }

    // Abre ou cria o arquivo de IDs
    FILE *idFile = fopen("last_id.txt", "r+");
    if (idFile == NULL) {
        idFile = fopen("last_id.txt", "w+");
        if (idFile == NULL) {
            printf("Erro: nao foi possivel criar o arquivo de IDs.\n");
            fclose(arquivo);
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

    // Preenche o registro do ingrediente
    ing.id = novoID;
    getchar(); // Limpa o buffer do teclado
    printf("Digite o nome: ");
    fgets(ing.nome, sizeof(ing.nome), stdin);
    ing.nome[strcspn(ing.nome, "\n")] = '\0'; // Remove o '\n' do final da string

    printf("Digite o preco: ");
    scanf("%f", &ing.preco);

    // Salva o ingrediente no arquivo de ingredientes
    fwrite(&ing, sizeof(ingrediente), 1, arquivo);
    fclose(arquivo);

    printf("Registro adicionado com ID: %d\n\n", ing.id);

    // Pausa para permitir a visualização
    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar();
}


void visualizarIngrediente() {
    FILE *arquivo = fopen("ingredientes.txt", "rb"); // Abre em modo leitura binária
    ingrediente ing;

    if (arquivo) {
        printf("\nIngredientes cadastrados:\n\n");
        while (fread(&ing, sizeof(ingrediente), 1, arquivo) == 1) {
            printf("ID: %d, Nome: %s, Preco: %.2f\n", ing.id, ing.nome, ing.preco);
        }
        fclose(arquivo);
    } else {
        printf("Nenhum ingrediente encontrado.\n");
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}


void editarIngrediente(){
    char nomeBusca[50];
    ingrediente ing;
    int encontrado = 0;

    printf("Digite o nome do ingrediente que deseja editar: ");
    getchar(); // Limpa o buffer
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0'; // Remove o '\n' do final da string

    FILE *arquivo = fopen("ingredientes.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    while (fread(&ing, sizeof(ingrediente), 1, arquivo) == 1) {
        if (strcmp(ing.nome, nomeBusca) == 0) {
            encontrado = 1;
            printf("Ingrediente encontrado:\n");
            printf("ID: %d, Nome: %s, Preco: %.2f\n", ing.id, ing.nome, ing.preco);

            printf("Digite o novo nome: ");
            fgets(ing.nome, sizeof(ing.nome), stdin);
            ing.nome[strcspn(ing.nome, "\n")] = '\0';

            printf("Digite o novo preco: ");
            scanf("%f", &ing.preco);
        }
        fwrite(&ing, sizeof(ingrediente), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove("ingredientes.txt");
        rename("temp.txt", "ingredientes.txt");
        printf("Ingrediente atualizado com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Ingrediente com nome '%s' nao encontrado.\n", nomeBusca);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}


void removerIngrediente(){
    char nomeBusca[50];
    ingrediente ing;
    int encontrado = 0;

    printf("Digite o nome do ingrediente que deseja deletar: ");
    getchar(); // Limpa o buffer
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0'; // Remove o '\n' do final da string

    FILE *arquivo = fopen("ingredientes.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    while (fread(&ing, sizeof(ingrediente), 1, arquivo) == 1) {
        if (strcmp(ing.nome, nomeBusca) == 0) {
            encontrado = 1;
            printf("Ingrediente '%s' deletado.\n", ing.nome);
            continue; // Ignora o ingrediente para não copiá-lo para o arquivo temporário
        }
        fwrite(&ing, sizeof(ingrediente), 1, temp);
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        remove("ingredientes.txt");
        rename("temp.txt", "ingredientes.txt");
    } else {
        remove("temp.txt");
        printf("Ingrediente com nome '%s' nao encontrado.\n", nomeBusca);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto

}