#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void _jugar_ahorcado(){
    char *cadena = NULL;
    int intentos = 3;
    ahorcado_t ahorcado;

    printf("Palabra a adivinar:\n");
    _leer_palabra(&cadena);        
    ahorcado_crear(&ahorcado,cadena,intentos);
    while (ahorcado_estado(&ahorcado)==0){
        free(cadena);
        _leer_palabra(&cadena);
        ahorcado_adivinar(&ahorcado,cadena[0]);
    }        
    free(cadena);    
    ahorcado_destruir(&ahorcado);
}

int main(){  
    // _jugar_ahorcado();
    socket_t socket;
    ssize_t bytes_enviados = 0;
    socket_crear(&socket);
    socket_conectar(&socket, "localhost", "7777");
    char buffer[] = "Hola Mundo\n";
    bytes_enviados = socket_enviar(&socket, buffer, strlen(buffer));            
    printf("%ld\n", bytes_enviados);
    socket_destruir(&socket);
    return 0;    
}

