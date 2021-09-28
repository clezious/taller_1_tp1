#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "common_socket.h"

typedef struct {
    bool juego_terminado;
    socket_t socket;
} cliente_t;

//Inicia el cliente, conectandose al host y servicio indicados
void cliente_crear(cliente_t *self, char *host, char *servicio);

//Destruye el cliente, cerrando la conexión al servidor
void cliente_destruir(cliente_t *self);

//Recibe la información de la partida en curso del servidor.
//A partir de esto, se define si el juego continúa o terminó 
//y se imprime por salida estandar la información pertinente
void cliente_recibir_estado(cliente_t *self);

//Envia la letra indicada al servidor
void cliente_enviar_letra(cliente_t *self, char letra);
#endif
