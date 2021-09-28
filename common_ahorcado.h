#ifndef __COMMON_AHORCADO_H__
#define __COMMON_AHORCADO_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char * palabra;
    char * palabra_adivinada;
    uint16_t longitud_palabra;
    uint8_t intentos;
    bool juego_finalizado;
    bool victoria;
} ahorcado_t;

//Inicializa el juego usando "palabra" como la palabra a adivinar 
//y con el numero máximo de intentos indicado.
void ahorcado_crear(ahorcado_t *self, char *palabra, uint8_t intentos);
//Destruye el juego, liberando las palabras almacenadas en memoria.
void ahorcado_destruir(ahorcado_t *self);
//Recibe una propuesta de letra indicada por parámetro y actualiza el estado 
//del juego dependiendo de si la letra estaba o no en la palabra a adivinar.
void ahorcado_adivinar(ahorcado_t *self, char letra);

#endif
