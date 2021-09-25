#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common_ahorcado.h"

void leer_palabra(char **cadena){
    size_t numero_bytes = 0;
    size_t bytes_leidos;
    char * ptr;

    bytes_leidos = getline(cadena, &numero_bytes, stdin);
    if( (ptr = strchr(*cadena, '\n')) != NULL){
        *ptr = '\0';
    }    
    if(bytes_leidos==-1){
        return;
    };    
}

int main(){        
    char *cadena = NULL;
    int intentos = 3;
    ahorcado_t ahorcado;

    puts("Palabra a adivinar:\n");
    leer_palabra(&cadena);        
    ahorcado_crear(&ahorcado,cadena,intentos);
    while (ahorcado_estado(&ahorcado)==0){
        free(cadena);
        leer_palabra(&cadena);
        ahorcado_adivinar(&ahorcado,cadena[0]);
    }        
    free(cadena);    
    ahorcado_destruir(&ahorcado);
    return 0;    
}

