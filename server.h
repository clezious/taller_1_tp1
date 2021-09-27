#ifndef __SERVER_H__
#define __SERVER_H__

#include "common_socket.h"
#include <stdio.h>

typedef struct {
    ahorcado_t ahorcado;
    socket_t socket;
    socket_t socket_cliente;
    FILE * archivo_palabras;
} servidor_t;

int servidor_crear(servidor_t *self, char *servicio, char *archivo_palabras);
void servidor_destruir(servidor_t *self);
void servidor_aceptar_cliente(servidor_t *self);
void servidor_desconectar_cliente(servidor_t *self);
int servidor_iniciar_juego(servidor_t *self, uint8_t intentos);
void servidor_finalizar_juego(servidor_t *self);
int servidor_juego_finalizado(servidor_t *self);
int servidor_cliente_gano(servidor_t *self);
void servidor_enviar_datos_cliente(servidor_t *self);
void servidor_recibir_datos_cliente(servidor_t *self);

#endif
