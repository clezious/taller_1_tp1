#include "common_socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#define MAX_REINTENTOS 5

void socket_crear(socket_t *self){    
    self->file_descriptor = socket(AF_INET, SOCK_STREAM, 0);    
}

void socket_destruir(socket_t *self){    
    close(self->file_descriptor);
}

void socket_conectar(socket_t *self, const char *host, const char *servicio){
    struct addrinfo *direccion;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    getaddrinfo(host, servicio, &hints, &direccion);            
    connect(self->file_descriptor, direccion->ai_addr, direccion->ai_addrlen);
    freeaddrinfo(direccion);
}

ssize_t socket_enviar(socket_t *self, const char *buffer, size_t longitud){    
    ssize_t bytes_enviados_totales = 0;
    ssize_t bytes_enviados;
    int reintentos = 0;
    while ((bytes_enviados_totales < longitud) 
            && (reintentos <= MAX_REINTENTOS)){
        bytes_enviados = send(self->file_descriptor,
                              &buffer[bytes_enviados_totales],
                              longitud - bytes_enviados_totales,
                              MSG_NOSIGNAL);
        if (bytes_enviados != -1){ // Si no hay error en el envío
            reintentos = 0;
            bytes_enviados_totales += bytes_enviados;
        } else {
            reintentos += 1;
        }
    }    
    return bytes_enviados_totales;
}
