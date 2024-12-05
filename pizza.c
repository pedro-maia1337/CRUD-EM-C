#include "pizza.h"
#include "ingrediente.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void adicionarPizza() {
    pizza piz;
    int ultimoID;
    int qtdIngredientesPadrao;

    FILE *arquivoPizza = fopen("pizzas.txt", "ab");
    if (arquivoPizza == NULL) {
        printf("Erro ao abrir o arquivo pizzas.txt.\n");
        return;
    }

    FILE *idFile = fopen("pizzas_id.txt", "r+");
    if (idFile == NULL) {
        idFile = fopen("pizzas_id.txt", "w+");
        if (idFile == NULL) {
            printf("Erro: não foi possível criar o arquivo de IDs.\n");
            fclose(arquivoPizza);
            return;
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
    rewind(idFile);
    fprintf(idFile, "%d", novoID);
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

    printf("Digite a quantidade de ingredientes padroes da pizza: ");
    scanf("%d", &qtdIngredientesPadrao);

    if (qtdIngredientesPadrao > 10) {
        printf("Erro: O número máximo de ingredientes padrão é 10.\n");
        return;
    }

    // Solicitar os ingredientes padrão
    piz.qtdIngredientes = qtdIngredientesPadrao; // Define a quantidade de ingredientes padrão
    getchar(); // Limpa o buffer

    for (int i = 0; i < qtdIngredientesPadrao; i++) {
        printf("Digite o nome do ingrediente %d: ", i + 1);
        fgets(piz.ingredientes[i].nome, sizeof(piz.ingredientes[i].nome), stdin);
        piz.ingredientes[i].nome[strcspn(piz.ingredientes[i].nome, "\n")] = '\0'; // Remove o '\n'
    }

    // Salva a pizza no arquivo
    fwrite(&piz, sizeof(pizza), 1, arquivoPizza);
    fclose(arquivoPizza);

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
     
        printf("Ingredientes:\n");
        for (int i = 0; i < piz.qtdIngredientes; i++) { // Usar piz.qtdIngredientes
            printf("Nome do ingrediente: %s\n", piz.ingredientes[i].nome);
        }

        printf("\n");
        printf("\n");
    }

    fclose(arquivoPizza);

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
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

    printf("Digite o ID da pizza a ser editada: ");
    scanf("%d", &idPizza);

    while (fread(&piz, sizeof(pizza), 1, arquivoPizza) == 1) {
        if (piz.id == idPizza) {
            found = 1;
            printf("Pizza com ID %d encontrada! Vamos editar os dados.\n", idPizza);

            // Edita o nome da pizza
            getchar(); // Limpa o buffer do teclado
            printf("Digite o novo nome da pizza: ");
            fgets(piz.nome, sizeof(piz.nome), stdin);
            piz.nome[strcspn(piz.nome, "\n")] = '\0'; // Remove o '\n'

            // Edita o tamanho da pizza
            printf("Digite o novo tamanho da pizza (P, M, G): ");
            scanf(" %c", &piz.tamanho);

            // Edita o preço base da pizza
            printf("Digite o novo preço base da pizza: ");
            scanf("%f", &piz.preco);

            // Edita os ingredientes
            printf("Quantos ingredientes padrão a pizza terá? (Máx: 10): ");
            scanf("%d", &piz.qtdIngredientes);

            if (piz.qtdIngredientes > 10) {
                printf("Erro: Número de ingredientes excede o limite permitido.\n");
                fclose(arquivoPizza);
                return;
            }

            getchar(); // Limpa o buffer após scanf
            for (int i = 0; i < piz.qtdIngredientes; i++) {
                printf("Digite o nome do ingrediente %d: ", i + 1);
                fgets(piz.ingredientes[i].nome, sizeof(piz.ingredientes[i].nome), stdin);
                piz.ingredientes[i].nome[strcspn(piz.ingredientes[i].nome, "\n")] = '\0'; // Remove o '\n'
            }

            // Reposiciona o ponteiro do arquivo para sobrescrever os dados
            fseek(arquivoPizza, -sizeof(pizza), SEEK_CUR);
            fwrite(&piz, sizeof(pizza), 1, arquivoPizza);

            printf("\nPizza com ID %d editada com sucesso!\n", idPizza);
            break;
        }

        if (!found) {
            printf("Pizza com id '%d' não encontrada.\n", idPizza);
        }

        fclose(arquivoPizza);

        printf("Pressione Enter para continuar...\n");
        getchar(); // Aguarda o Enter
        getchar(); // Evita que o Enter residual da entrada anterior passe direto
    }

    if (!found) {
        printf("Pizza com ID %d não encontrada.\n", idPizza);
    }

    fclose(arquivoPizza);

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}


void removerPizza(){
    pizza piz;
    int idPizza;
    int found = 0;


    FILE *arquivoPizza = fopen("pizzas.txt", "rb");
    FILE *arquivoTemp = fopen("pizzas_temp.txt", "wb");

    if (arquivoPizza == NULL || arquivoTemp == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    printf("Digite o ID da pizza a ser excluida: ");
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
        printf("Pizza com ID '%d' não encontrada.\n", idPizza);
    }

    printf("Pressione Enter para continuar...\n");
    getchar(); // Aguarda o Enter
    getchar(); // Evita que o Enter residual da entrada anterior passe direto
}
