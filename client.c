#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "common_ahorcado.h"
#include "common_socket.h"

void _leer_palabra(char **cadena){
    size_t numero_bytes = 0;
    size_t bytes_leidos;
    char * ptr;

    bytes_leidos = getline(cadena, &numero_bytes, stdin);
    if ((ptr = strchr(*cadena, '\n')) != NULL){
        *ptr = '\0';
    }    
    if (bytes_leidos==-1){
        return;
    };    
}

int main(){  
    // _jugar_ahorcado();
    socket_t socket;
    ssize_t bytes_enviados = 0;
    socket_crear(&socket);
    socket_conectar(&socket, "localhost", "7776");
    char *buffer = NULL;    
    bool juego_terminado = false;
    uint8_t primer_byte = 0;
    uint16_t longitud_mensaje;
    char * palabra;
    while (juego_terminado==false){
        _leer_palabra(&buffer);
        for (int i = 0; i < strlen(buffer); i++){
            bytes_enviados += socket_enviar(&socket, &buffer[i], 1);            
            socket_recibir(&socket, (char*)&primer_byte, 1);
            socket_recibir(&socket, (char*)&longitud_mensaje, 2);
            palabra = calloc(longitud_mensaje + 1, sizeof(char));
            socket_recibir(&socket, palabra, longitud_mensaje);
            if (primer_byte >= 128){ //La partida terminó
                juego_terminado = true;
                if (primer_byte==128){ //No quedaban intentos: perdió
                    printf("Perdiste! La palabra secreta era: %s\n",palabra);
                } else {
                    printf("Ganaste!!\n");
                }
            } else{
                printf("Palabra secreta: %s\n", palabra);
                printf("Te quedan %d intentos\n",primer_byte);
                printf("Ingrese letra:\n");
            }
            free(palabra);
        };
        free(buffer);        
    }
    
    // bytes_enviados = socket_enviar(&socket, buffer, strlen(buffer));        
    // printf("%ld\n", bytes_enviados);
    socket_destruir(&socket);
    return 0;    
}

