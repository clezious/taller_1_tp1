#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#include <unistd.h>

typedef struct {
    int file_descriptor;
} socket_t;

//Inicia el socket
void socket_crear(socket_t *self);
//Destruye el socket
void socket_destruir(socket_t *self);

//Realiza un bind y listen en en host y servicio indicados.
void socket_escuchar(socket_t *self, const char *host, const char *servicio);
//Acepta una conexión del pool de conexiones y se la asigna al socket cliente.
void socket_aceptar_conexion(socket_t *self, socket_t *cliente);
//Abre una conexión al host y servicio indicados.
void socket_conectar(socket_t *self, const char *host, const char *servicio);
//Envia longitud bytes del buffer al socket.

ssize_t socket_enviar(socket_t *self, const char *buffer, size_t longitud);
//Recibe longitud bytes del socket y los guarda en buffer. 
ssize_t socket_recibir(socket_t *self, char *buffer, size_t longitud);

#endif
