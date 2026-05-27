#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "validaciones.h"


void validarContrasena(char *buffer, int tamBuffer) {
    do {
        printf("Ingrese la contrasena (minimo 8 caracteres): ");
        fgets(buffer, tamBuffer, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) < 8)
            printf("Error: la contrasena debe tener al menos 8 caracteres.\n");
    } while (strlen(buffer) < 8);
}
