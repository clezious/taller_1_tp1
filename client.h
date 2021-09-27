#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common_socket.h"

typedef struct {
    bool juego_terminado;
    socket_t socket;
} cliente_t;

void cliente_crear(cliente_t *self, char *host, char *servicio);

void cliente_destruir(cliente_t *self);

void cliente_recibir_de_servidor(cliente_t *self);

#endif
