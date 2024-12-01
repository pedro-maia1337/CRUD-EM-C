#ifndef INGREDIENTE_H_INCLUDED 
#define INGREDIENTE_H_INCLUDED

typedef struct ingredientes {
    int id;
    char nome[255];
    float preco;
} ingrediente;

void adicionarIngrediente();

void visualizarIngrediente();

void editarIngrediente();

void removerIngrediente();

#endif
