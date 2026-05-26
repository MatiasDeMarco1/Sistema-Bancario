#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cuenta.h"

Cuenta* crearCuenta() {
    Cuenta *cuenta = calloc(1, sizeof(Cuenta)); // aloja y pone todo en 0
    if (cuenta == NULL) {
        printf("Error: no se pudo alojar memoria.\n");
        return NULL;
    }

    char buffer[100];

    printf("Ingrese el CBU de la cuenta: ");
    scanf("%d", &cuenta->cbu);
    getchar(); // Consume the newline character left by scanf

    printf("Ingrese el alias de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cuenta->alias = malloc(strlen(buffer) + 1); // +1 para el '\0'
    strcpy(cuenta->alias, buffer);

    printf("Ingrese el nombre del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cuenta->nombre = malloc(strlen(buffer) + 1);
    strcpy(cuenta->nombre, buffer);

    printf("Ingrese el apellido del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cuenta->apellido = malloc(strlen(buffer) + 1);
    strcpy(cuenta->apellido, buffer);

    printf("Ingrese el telefono del titular de la cuenta: ");
    scanf("%ld", &cuenta->telefono);
    getchar(); // Consume the newline character left by scanf

    printf("Ingrese el mail del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    cuenta->mail = malloc(strlen(buffer) + 1);
    strcpy(cuenta->mail, buffer);

    printf("Ingrese el CUIT del titular de la cuenta: ");
    scanf("%ld", &cuenta->cuit);
    getchar(); // Consume the newline character left by scanf

    return cuenta;
}

void eliminarCuenta(Cuenta **cuenta) {
    if (cuenta == NULL || *cuenta == NULL) {
        return;
    }
    free((*cuenta)->alias);
    free((*cuenta)->nombre);
    free((*cuenta)->apellido);
    free((*cuenta)->mail);
    free(*cuenta);
    *cuenta = NULL;
}

void mostrarCuenta(Cuenta *cuenta) {
    if (cuenta == NULL) {
        printf("La cuenta no existe.\n");
        return;
    }
    printf("CBU: %d\n", cuenta->cbu);
    printf("Alias: %s\n", cuenta->alias);
    printf("Nombre: %s\n", cuenta->nombre);
    printf("Apellido: %s\n", cuenta->apellido);
    printf("Telefono: %ld\n", cuenta->telefono);
    printf("Mail: %s\n", cuenta->mail);
    printf("CUIT: %ld\n", cuenta->cuit);
}

void editarCuenta(Cuenta *cuenta) {
    if (cuenta == NULL) {
        printf("La cuenta no existe.\n");
        return;
    }
    int opcion;
    printf("Seleccione el campo a editar:\n");
    printf("1. Alias\n");
    printf("2. Nombre\n");
    printf("3. Apellido\n");
    printf("4. Telefono\n");
    printf("5. Mail\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar(); // Consume the newline character left by scanf

    switch (opcion) {
        case 1:
            editarAlias(cuenta);
            break;
        case 2:
            editarNombre(cuenta);
            break;
        case 3:
            editarApellido(cuenta);
            break;
        case 4:
            editarTelefono(cuenta);
            break;
        case 5:
            editarMail(cuenta);
            break;
        default:
            printf("Opcion invalida.\n");
    }
}

void editarAlias(Cuenta *cuenta) {
    char buffer[100];
    printf("Ingrese el nuevo alias de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    free(cuenta->alias); // Liberar el alias anterior
    cuenta->alias = malloc(strlen(buffer) + 1);
    strcpy(cuenta->alias, buffer);
}

void editarNombre(Cuenta *cuenta) {
    char buffer[100];
    printf("Ingrese el nuevo nombre del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    free(cuenta->nombre); // Liberar el nombre anterior
    cuenta->nombre = malloc(strlen(buffer) + 1);
    strcpy(cuenta->nombre, buffer);
}

void editarApellido(Cuenta *cuenta) {
    char buffer[100];
    printf("Ingrese el nuevo apellido del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    free(cuenta->apellido); // Liberar el apellido anterior
    cuenta->apellido = malloc(strlen(buffer) + 1);
    strcpy(cuenta->apellido, buffer);
}

void editarTelefono(Cuenta *cuenta) {
    printf("Ingrese el nuevo telefono del titular de la cuenta: ");
    scanf("%ld", &cuenta->telefono);
    getchar(); // Consume the newline character left by scanf
}

void editarMail(Cuenta *cuenta) {
    char buffer[100];
    printf("Ingrese el nuevo mail del titular de la cuenta: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    free(cuenta->mail); // Liberar el mail anterior
    cuenta->mail = malloc(strlen(buffer) + 1);
    strcpy(cuenta->mail, buffer);
}