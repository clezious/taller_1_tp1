#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "common_ahorcado.h"

void ahorcado_crear(ahorcado_t *self, char *palabra, int intentos){
    self->intentos = intentos;
    self->juego_finalizado = false;
    self->palabra = malloc(strlen(palabra)+1);
    strcpy(self->palabra,palabra);
    self->palabra_adivinada = malloc(strlen(palabra)+1);
    strcpy(self->palabra_adivinada,palabra);
    for (int i = 0; i < strlen(self->palabra); i++){
        self->palabra_adivinada[i]='_';
    };    
}
void ahorcado_destruir(ahorcado_t *self){
    free(self->palabra);
    free(self->palabra_adivinada);
}
void ahorcado_adivinar(ahorcado_t *self, char letra){
    if (self->juego_finalizado == true){
        //Si el juego ya terminó, no puede seguir adivinando
        return; 
    }
    bool acierto = false;
    for (int i = 0; i < strlen(self->palabra); i++){
        if (self->palabra[i] == letra){
            self->palabra_adivinada[i] = letra;
            acierto = true;
        };
    };
    if (strcmp(self->palabra_adivinada,self->palabra)==0){
        //Si la palabra adivinada es igual a la palabra, entonces ganó.
        self->juego_finalizado = true;
        return;
    }
    if (acierto == false){
        //Si no acertó, se descuentan intentos
        self->intentos = self->intentos - 1;
        if (self->intentos == 0){
            self->juego_finalizado = true;
        }
    };
}
int ahorcado_estado(ahorcado_t *self){
    printf("%s\n",self->palabra);
    printf("%s\n",self->palabra_adivinada);
    printf("%d\n",self->intentos);
    printf("%d\n",self->juego_finalizado);
    if (self->juego_finalizado){
        return 1;
    }
    return 0;
}