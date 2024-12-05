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
        fprintf(idFile, "0"); 
        fflush(idFile);
        rewind(idFile);
    }

    if (fscanf(idFile, "%d", &ultimoID) != 1) { // Lê o último ID
        ultimoID = 0; 
    }

    
    int novoID = ultimoID + 1; // Incrementa o ID

    rewind(idFile); // Move o cursor para o início do arquivo
    fprintf(idFile, "%d", novoID); // Grava o novo ID
    fflush(idFile);
    fclose(idFile);

    //Pede os dados
    ing.id = novoID;
    getchar(); 
    printf("Digite o nome: ");
    fgets(ing.nome, sizeof(ing.nome), stdin);
    ing.nome[strcspn(ing.nome, "\n")] = '\0'; 

    printf("Digite o preco: ");
    scanf("%f", &ing.preco);

    
    fwrite(&ing, sizeof(ingrediente), 1, arquivo); // Salva o ingrediente no arquivo de ingredientes
    fclose(arquivo); //Fecha o arquivo

    printf("Registro adicionado com ID: %d\n\n", ing.id);

    printf("Pressione Enter para continuar...\n");
    getchar(); 
    getchar();
}


void visualizarIngrediente() {
    FILE *arquivo = fopen("ingredientes.txt", "rb"); // Abre em modo leitura 
    ingrediente ing;

    //Lendo o arquivo
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
    getchar(); 
    getchar(); 
}


void editarIngrediente(){
    char nomeBusca[50];
    ingrediente ing;
    int encontrado = 0;
    int idIngrediente;

    getchar(); 
    printf("Digite o ID do ingrediente que deseja alterar: "); //Pergunta o id para busca da pizza no arquivo
    scanf("%d", &idIngrediente);

    FILE *arquivo = fopen("ingredientes.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    //Pedindo os novos dados
    while (fread(&ing, sizeof(ingrediente), 1, arquivo) == 1) {
        if (ing.id == idIngrediente) {
            encontrado = 1;
             
            printf("Ingrediente encontrado:\n");
            printf("ID: %d, Nome: %s, Preco: %.2f\n", ing.id, ing.nome, ing.preco);

            getchar();
            printf("Digite o novo nome: ");
            fgets(ing.nome, sizeof(ing.nome), stdin);
            ing.nome[strcspn(ing.nome, "\n")] = '\0';

            printf("Digite o novo preco: ");
            scanf("%f", &ing.preco);
        }
        fwrite(&ing, sizeof(ingrediente), 1, temp); // Copia a pizza para o arquivo temporário
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        // Substitui o arquivo original pelo arquivo temporário
        remove("ingredientes.txt");
        rename("temp.txt", "ingredientes.txt");
        printf("Ingrediente atualizado com sucesso.\n");
    } else {
        remove("temp.txt");
        printf("Ingrediente com ID'%d' nao encontrado.\n", idIngrediente);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); 
    getchar(); 
}


void removerIngrediente(){
    char nomeBusca[50];
    ingrediente ing;
    int encontrado = 0;
    int idIngrediente;

    printf("Digite o ID do ingrediente que deseja deletar: "); //Pergunta o id para busca da pizza no arquivo
    scanf("%d", &idIngrediente);

    //Abre os arquivos
    FILE *arquivo = fopen("ingredientes.txt", "rb");
    FILE *temp = fopen("temp.txt", "wb");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        if (arquivo) fclose(arquivo);
        if (temp) fclose(temp);
        return;
    }

    while (fread(&ing, sizeof(ingrediente), 1, arquivo) == 1) {
        if (ing.id == idIngrediente) {
            encontrado = 1;
            printf("Ingrediente '%s' deletado.\n", ing.nome);
            continue; // Ignora o ingrediente para não copiá-lo para o arquivo temporário
        }
        fwrite(&ing, sizeof(ingrediente), 1, temp); // Copia a pizza para o arquivo temporário
    }

    fclose(arquivo);
    fclose(temp);

    if (encontrado) {
        // Substitui o arquivo original pelo arquivo temporário
        remove("ingredientes.txt");
        rename("temp.txt", "ingredientes.txt");
    } else {
        remove("temp.txt");
        printf("Ingrediente com nome '%d' nao encontrado.\n", idIngrediente);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto

}