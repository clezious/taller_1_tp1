#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_ahorcado.h"
#include "common_socket.h"

int main(){  
    // _jugar_ahorcado();
    socket_t socket, socket_cliente;
    ssize_t bytes_recibidos = 0;
    char buffer[20] = {0};    
    socket_crear(&socket);
    socket_escuchar(&socket, "localhost", "7777");
    socket_aceptar_conexion(&socket, &socket_cliente);
    bytes_recibidos = socket_recibir(&socket_cliente, buffer, sizeof(buffer)-1);
    printf("Bytes recibidos: %ld\n", bytes_recibidos);
    printf("Buffer: %s\n", buffer);
    socket_destruir(&socket_cliente);
    socket_destruir(&socket);
    return 0;    
}
