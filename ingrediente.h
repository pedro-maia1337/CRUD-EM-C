#ifndef INGREDIENTE_H_INCLUDED 
#define INGREDIENTE_H_INCLUDED

//Estrutura dos ingredientes
typedef struct ingredientes {
    int id;
    char nome[255];
    float preco;
} ingrediente;

//Declarações das funções 
void adicionarIngrediente();
void visualizarIngrediente();
void editarIngrediente();
void removerIngrediente();

#endif