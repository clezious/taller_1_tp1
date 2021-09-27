#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "common_ahorcado.h"
#include "common_socket.h"


int main(){  
    // _jugar_ahorcado();
    socket_t socket, socket_cliente;
    ssize_t bytes_recibidos = 0;
    char letra = 0;    

    socket_crear(&socket);
    socket_escuchar(&socket, "localhost", "7777");
    socket_aceptar_conexion(&socket, &socket_cliente);
    
    ahorcado_t ahorcado;
    uint8_t intentos = 3;
    char *palabra = "pepardo";
    uint16_t longitud_palabra = strlen(palabra);
    if (intentos >= 128 || longitud_palabra >= 65536){
        return 1; //Valores máximos para estas longitudes por consigna.
    }
    
    ahorcado_crear(&ahorcado,palabra,intentos);    
        
    uint8_t primer_byte = 0;
    char *palabra_a_enviar;
    while (ahorcado.juego_finalizado == 0){
        bytes_recibidos += socket_recibir(&socket_cliente, &letra, sizeof(char));        
        ahorcado_adivinar(&ahorcado,letra);
        // printf("%c\n",letra);
        
        primer_byte = ahorcado.intentos;
        palabra_a_enviar = ahorcado.palabra_adivinada;
        if (ahorcado.juego_finalizado != 0){
            primer_byte += 128;
            palabra_a_enviar = ahorcado.palabra;
        }        
        socket_enviar(&socket_cliente, (char*)&primer_byte, 1);
        socket_enviar(&socket_cliente, (char*)&longitud_palabra, 2);
        socket_enviar(&socket_cliente, palabra_a_enviar, longitud_palabra);        
    }
    ahorcado_destruir(&ahorcado);    
    // printf("Bytes recibidos: %ld\n", bytes_recibidos);
    
    socket_destruir(&socket_cliente);
    socket_destruir(&socket);
    return 0;    
}
