#ifndef PIZZA_H_INCLUDED
#define PIZZA_H_INCLUDED
#include "ingrediente.h"

typedef struct pizzas {
    int id;
    char nome[255];
    char tamanho;
    float preco;
    ingrediente ingredientes[10];
    int qtdIngredientes;
} pizza;

void adicionarPizza();

void visualizarPizza();

void editarPizza();

void removerPizza();

#endif
