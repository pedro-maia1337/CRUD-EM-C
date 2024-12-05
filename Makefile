# Nome do compilador
CC = gcc

# Opções de compilação
CFLAGS = -Wall -g

# Arquivos de origem
SRC = main.c pizza.c ingrediente.c

# Arquivos de cabeçalho
HEADERS = pizza.h ingrediente.h

# Arquivos objeto gerados
OBJ = main.o pizza.o ingrediente.o

# Nome do executável final
TARGET = pizzaria.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	del $(OBJ) $(TARGET)


run: $(TARGET)
	./$(TARGET)