#ifndef __COMMON_AHORCADO_H__
#define __COMMON_AHORCADO_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char * palabra;
    char * palabra_adivinada;
    uint8_t intentos;
    bool juego_finalizado;
} ahorcado_t;

void ahorcado_crear(ahorcado_t *self, char *palabra, uint8_t intentos);
void ahorcado_destruir(ahorcado_t *self);
void ahorcado_adivinar(ahorcado_t *self, char letra);

#endif
