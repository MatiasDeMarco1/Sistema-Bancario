#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "login.h"

void verificarIngreso(Cliente *c) {
    char buffer[100];
    char contrasena[100];
    int opcion;

    printf("Ingrese con que opcion desea ingresar: ");
    printf("1. Mail\n");
    printf("2. CUIT\n");
    printf("3. Telefono\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar(); // Consume the newline character left by scanf

    switch (opcion) {
        case 1:
            printf("Ingrese el mail: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
            break;
        case 2:
            printf("Ingrese el CUIT: ");
            scanf("%ld", &c->cuit);
            getchar(); // Consume the newline character left by scanf
            break;
        case 3:
            printf("Ingrese el telefono: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
            break;
        default:
            printf("Opcion invalida.\n");
    }

    printf("Ingrese la contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0'; // Remove newline character

    if ((strcmp(buffer, c->mail) == 0 || atol(buffer) == c->cuit || atol(buffer) == c->telefono) && strcmp(contrasena, c->contrasena) == 0) {
        printf("Ingreso exitoso.\n");
    } else {
        printf("Ingreso fallido. Credenciales incorrectas.\n");
    }
}