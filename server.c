#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "common_ahorcado.h"
#include "common_socket.h"
#include "server.h"

int servidor_crear(servidor_t *self, char *servicio, char *archivo_palabras){
    if ((self->archivo_palabras = fopen(archivo_palabras,"r")) == NULL){
        return 1;
    }
    socket_crear(&self->socket);
    socket_escuchar(&self->socket, "localhost", servicio);
    return 0;  
}
void servidor_destruir(servidor_t *self){
    socket_destruir(&self->socket);
    fclose(self->archivo_palabras);
}
void servidor_aceptar_cliente(servidor_t *self){
    socket_aceptar_conexion(&self->socket, &self->socket_cliente);
}
void servidor_desconectar_cliente(servidor_t *self){
    socket_destruir(&self->socket_cliente);
}
int servidor_iniciar_juego(servidor_t *self, uint8_t intentos){   
    //Se lee una palabra (si hay) y se inicia el juego.
    //Si no hay una nueva palabra, devuelve 1    
    size_t bytes_palabra = 0;
    char *palabra = NULL;
    if (getline(&palabra, &bytes_palabra, self->archivo_palabras) != -1){
        // Longitud máxima de palabra por consigna (16 bits)            
        if (bytes_palabra >= 65536){
            free(palabra);
            return -1;
        }
        char * ptr;
        if ((ptr = strchr(palabra, '\n')) != NULL){
            *ptr = '\0';
        }
        // printf("PALABRA LEIDA: %s\n",palabra);        
        ahorcado_crear(&self->ahorcado,palabra,intentos);
        free(palabra);
        return 0;
    }
    free(palabra);
    return -1;    
}
void servidor_finalizar_juego(servidor_t *self){
    ahorcado_destruir(&self->ahorcado);
}
int servidor_juego_finalizado(servidor_t *self){
    return self->ahorcado.juego_finalizado;
}
int servidor_cliente_gano(servidor_t *self){
    return self->ahorcado.victoria;
}
void servidor_enviar_datos_cliente(servidor_t *self){
    uint8_t primer_byte = 0;
    char *palabra_a_enviar;

    primer_byte = self->ahorcado.intentos;
    palabra_a_enviar = self->ahorcado.palabra_adivinada;
    if (servidor_juego_finalizado(self) != 0){
        primer_byte += 128;
        palabra_a_enviar = self->ahorcado.palabra;
    }
    socket_enviar(&self->socket_cliente, (char*)&primer_byte, 1);
    unsigned short longitud_palabra = htons(self->ahorcado.longitud_palabra);
    socket_enviar(&self->socket_cliente,
                  (char*)&longitud_palabra,
                  2);
    socket_enviar(&self->socket_cliente,
                  palabra_a_enviar,
                  self->ahorcado.longitud_palabra);
}
void servidor_recibir_datos_cliente(servidor_t *self){
    char letra = 0;
    socket_recibir(&self->socket_cliente, &letra, sizeof(char));
    ahorcado_adivinar(&self->ahorcado,letra);
}

int main(int argc, char *argv[]){
    if (argc != 4){
        return 1; //ERROR: Faltan parámetros
    }
    // printf("FILE NAME: %s\n",argv[3]);  
    long intentos = strtol(argv[2], NULL, 10);
    if (intentos >= 128){
        return 1; //Valor máximo para intentos por consigna (7bits).
    }
    
    servidor_t servidor;
    int partidas = 0;
    int victorias = 0;
    if (servidor_crear(&servidor, argv[1], argv[3]) != 0){
        return 1;
    }
    while (servidor_iniciar_juego(&servidor,(uint8_t)intentos) == 0){
        servidor_aceptar_cliente(&servidor);
        partidas += 1;
        servidor_enviar_datos_cliente(&servidor);
        while (servidor_juego_finalizado(&servidor) == 0){        
            servidor_recibir_datos_cliente(&servidor);
            servidor_enviar_datos_cliente(&servidor);
        }
        victorias += servidor_cliente_gano(&servidor);
        servidor_desconectar_cliente(&servidor);
        servidor_finalizar_juego(&servidor);
    }
    printf("Resumen:\n");
    printf("\tVictorias: %d\n",victorias);
    printf("\tDerrotas: %d\n",(partidas - victorias));
    servidor_destruir(&servidor);
    return 0;    
}
