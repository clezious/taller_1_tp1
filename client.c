#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "common_ahorcado.h"
#include "common_socket.h"
#include "client.h"

//lee una palabra de stdin y le quiza el \n
void _leer_palabra(char **cadena){
    size_t bytes_linea = 0;
    ssize_t bytes_leidos;
    char * ptr;

    bytes_leidos = getline(cadena, &bytes_linea, stdin);
    if ((ptr = strchr(*cadena, '\n')) != NULL){
        *ptr = '\0';
    }    
    if (bytes_leidos==-1){
        return;
    }    
}

void cliente_crear(cliente_t *self, char *host, char *servicio){
    socket_t socket;
    socket_crear(&socket);
    socket_conectar(&socket, host, servicio);
    self->socket = socket;
    
    self->juego_terminado = false;
}

void cliente_destruir(cliente_t *self){        
    socket_destruir(&self->socket);    
}

void cliente_recibir_de_servidor(cliente_t *self){
    uint8_t primer_byte = 0;
    uint16_t longitud_mensaje;
    char * palabra;
    socket_recibir(&self->socket, (char*)&primer_byte, 1);
    socket_recibir(&self->socket, (char*)&longitud_mensaje, 2);
    longitud_mensaje = ntohs(longitud_mensaje);
    palabra = calloc(longitud_mensaje + 1, sizeof(char));
    socket_recibir(&self->socket, palabra, longitud_mensaje);
    if (primer_byte >= 128){ //La partida terminó
        self->juego_terminado = true;
        if (primer_byte==128){ //No quedaban intentos: perdió
            printf("Perdiste! La palabra secreta era: '%s'\n",palabra);
        } else {
            printf("Ganaste!!\n");
        }
    } else{
        printf("Palabra secreta: %s\n", palabra);
        printf("Te quedan %d intentos\n",primer_byte);
        printf("Ingrese letra:\n");
    }
    free(palabra);
}

int main(int argc, char *argv[]){ 
    if (argc != 3){
        return 1; //ERROR: Faltan parámetros
    }
    char *buffer = NULL;        
    cliente_t cliente;
    cliente_crear(&cliente, argv[1], argv[2]);
    cliente_recibir_de_servidor(&cliente);
    while (cliente.juego_terminado==false){
        _leer_palabra(&buffer);
        for (int i = 0; i < strlen(buffer); i++){
            socket_enviar(&cliente.socket, &buffer[i], 1);            
            cliente_recibir_de_servidor(&cliente);            
        }
        free(buffer);        
    }    
    cliente_destruir(&cliente);    
    return 0;    
}

