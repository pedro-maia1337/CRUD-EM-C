#ifndef PIZZA_H_INCLUDED
#define PIZZA_H_INCLUDED
#include "ingrediente.h"


//Estrutura das pizzas
typedef struct pizzas {
    int id;
    char nome[255];
    char tamanho;
    float preco;
    ingrediente ingredientes[10];
} pizza;

//Declarações das funções 
void adicionarPizza();
void visualizarPizza();
void editarPizza();
void removerPizza();
void venderPizza();

#endif