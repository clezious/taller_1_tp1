#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <unistd.h>

typedef struct {
    int file_descriptor;
} socket_t;

void socket_crear(socket_t *self);
void socket_destruir(socket_t *self);

void socket_escuchar(socket_t *self, const char *host, const char *servicio);
void socket_aceptar_conexion(socket_t *self, socket_t *cliente);

void socket_conectar(socket_t *self, const char *host, const char *servicio);

ssize_t socket_enviar(socket_t *self, const char *buffer, size_t longitud);
ssize_t socket_recibir(socket_t *self, char *buffer, size_t longitud);

#endif
