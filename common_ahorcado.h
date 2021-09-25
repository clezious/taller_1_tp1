#ifndef __COMMON_AHORCADO_H__
#define __COMMON_AHORCADO_H__

#include <stdbool.h>

typedef struct {
    char * palabra;
    char * palabra_adivinada;
    int intentos;
    bool juego_finalizado;
} ahorcado_t;

void ahorcado_crear(ahorcado_t *self, char *palabra, int intentos);
void ahorcado_destruir(ahorcado_t *self);
void ahorcado_adivinar(ahorcado_t *self, char letra);
int ahorcado_estado(ahorcado_t *self);

#endif