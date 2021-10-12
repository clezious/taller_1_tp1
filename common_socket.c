#include "common_socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define MAX_POOL_CONEXIONES 32

// Guarda en direccion un addrinfo con el host y servicio indicados
// servidor es un bool que indica si la direccion es 
// para un servidor o un cliente
// si servidor es true, se utiliza el flag AI_PASSIVE, si no, NULL.
static void _getaddrinfo(struct addrinfo **direccion, 
                  const char *host, 
                  const char * servicio, 
                  bool servidor){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (servidor == true){
        hints.ai_flags = AI_PASSIVE;
    }
    getaddrinfo(host, servicio, &hints, direccion);
}

void socket_crear(socket_t *self){    
    self->file_descriptor = socket(AF_INET, SOCK_STREAM, 0);    
}

void socket_destruir(socket_t *self){    
    shutdown(self->file_descriptor,SHUT_RDWR);
    close(self->file_descriptor);
}

void socket_conectar(socket_t *self, const char *host, const char *servicio){
    struct addrinfo *direccion, *puntero;
    bool conexion_exitosa = false;
    _getaddrinfo(&direccion, host, servicio, false);
    for (puntero = direccion; 
         puntero != NULL && conexion_exitosa == false; 
         puntero = puntero->ai_next) {
        // Se recorren las direcciones intentando conectar hasta que lo logre (o no). 
        if (connect(self->file_descriptor, puntero->ai_addr, puntero->ai_addrlen) != -1) {            
            conexion_exitosa = true;
        }
    }
    freeaddrinfo(direccion);
}

void socket_escuchar(socket_t *self, const char *host, const char *servicio){
    struct addrinfo *direccion;
    _getaddrinfo(&direccion, host, servicio, true);    
    bind(self->file_descriptor, direccion->ai_addr, direccion->ai_addrlen);
    freeaddrinfo(direccion);
    listen(self->file_descriptor,MAX_POOL_CONEXIONES);
}

void socket_aceptar_conexion(socket_t *self, socket_t *cliente){    
    cliente->file_descriptor = accept(self->file_descriptor, NULL, NULL);
}

ssize_t socket_enviar(socket_t *self, const char *buffer, size_t longitud){    
    ssize_t bytes_enviados_totales = 0;    
    while (bytes_enviados_totales < longitud){
        ssize_t bytes_enviados = send(self->file_descriptor,
                              &buffer[bytes_enviados_totales],
                              longitud - bytes_enviados_totales,
                              MSG_NOSIGNAL);
        if (bytes_enviados != -1){
            bytes_enviados_totales += bytes_enviados;
        }
    }    
    return bytes_enviados_totales;
}

ssize_t socket_recibir(socket_t *self, char *buffer, size_t longitud){
    ssize_t bytes_recibidos_totales = 0;     
    while ((bytes_recibidos_totales < longitud)){
        ssize_t bytes_recibidos = recv(self->file_descriptor,
                              &buffer[bytes_recibidos_totales],
                              longitud - bytes_recibidos_totales,
                              0);
        bytes_recibidos_totales += bytes_recibidos;        
    }    
    return bytes_recibidos_totales;
}
