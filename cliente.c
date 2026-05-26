#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"


Cliente* crearCliente() {
    Cliente *c = calloc(1, sizeof(Cliente)); // aloja y pone todo en 0
    if (c == NULL) {
        printf("Error: no se pudo alojar memoria.\n");
        return NULL;
    }

    char buffer[100];

    printf("Ingrese el nombre del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->nombre = malloc(strlen(buffer) + 1); // +1 para el '\0'
    strcpy(c->nombre, buffer);

    printf("Ingrese el apellido del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->apellido = malloc(strlen(buffer) + 1);
    strcpy(c->apellido, buffer);

    printf("Ingrese la edad del cliente: ");
    scanf("%d", &c->edad);
    getchar(); // Consume the newline character left by scanf

    printf("Ingrese el CUIT del cliente: ");
    scanf("%ld", &c->cuit);
    getchar(); // Consume the newline character left by scanf

    printf("Ingrese el mail del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->mail = malloc(strlen(buffer) + 1);
    strcpy(c->mail, buffer);

    printf("Ingrese el telefono del cliente: ");
    scanf("%ld", &c->telefono);
    getchar(); // Consume the newline character left by scanf

    printf("Ingrese la localidad del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->localidad = malloc(strlen(buffer) + 1);
    strcpy(c->localidad, buffer);

    printf("Ingrese el pais del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->pais = malloc(strlen(buffer) + 1);
    strcpy(c->pais, buffer);

    printf("Ingrese la contrasena del cliente: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    c->contrasena = malloc(strlen(buffer) + 1);
    strcpy(c->contrasena, buffer);

    return c;  
}

void eliminarCliente(Cliente **c) {
    if (*c == NULL) return;

    free((*c)->nombre);
    free((*c)->apellido);
    free((*c)->mail);
    free((*c)->localidad);
    free((*c)->pais);
    free((*c)->contrasena);

    free(*c);       // libera la estructura en sí
    *c = NULL;      // evita puntero colgante
}


void editarCliente(Cliente *c) {
    int opcion;
    printf("Seleccione el campo a editar:\n");
    printf("1. Nombre\n");
    printf("2. Apellido\n");
    printf("3. Edad\n");
    printf("4. CUIT\n");
    printf("5. Mail\n");
    printf("6. Telefono\n");
    printf("7. Localidad\n");
    printf("8. Pais\n");
    printf("9. Contrasena\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar(); // Consume the newline character left by scanf
    switch (opcion) {
        case 1:
            editarNombre(c); break;
        case 2:
            editarApellido(c); break;
        case 3:
            editarEdad(c); break;
        case 4:
            editarCUIT(c); break;
        case 5:
            editarMail(c); break;
        case 6:
            editarTelefono(c); break;
        case 7:
            editarLocalidad(c); break;
        case 8:
            editarPais(c); break;
        case 9:
            editarContrasena(c); break;
        default:
            printf("Opcion invalida.\n");   
    }
}

void editarNombre(Cliente *c) {
    printf("Ingrese el nuevo nombre del cliente: ");
    fgets(c->nombre, sizeof(c->nombre), stdin);
    c->nombre[strcspn(c->nombre, "\n")] = '\0'; // Remove newline character
}

void editarApellido(Cliente *c) {
    printf("Ingrese el nuevo apellido del cliente: ");
    fgets(c->apellido, sizeof(c->apellido), stdin);
    c->apellido[strcspn(c->apellido, "\n")] = '\0'; // Remove newline character
}

void editarEdad(Cliente *c) {
    printf("Ingrese la nueva edad del cliente: ");
    scanf("%d", &c->edad);
    getchar(); // Consume the newline character left by scanf
}

void editarCUIT(Cliente *c) {
    printf("Ingrese el nuevo CUIT del cliente: ");
    scanf("%ld", &c->cuit);
    getchar(); // Consume the newline character left by scanf
}

void editarMail(Cliente *c) {
    printf("Ingrese el nuevo mail del cliente: ");
    fgets(c->mail, sizeof(c->mail), stdin);
    c->mail[strcspn(c->mail, "\n")] = '\0'; // Remove newline character
}

void editarTelefono(Cliente *c) {
    printf("Ingrese el nuevo telefono del cliente: ");
    scanf("%ld", &c->telefono);
    getchar(); // Consume the newline character left by scanf
}

void editarLocalidad(Cliente *c) {
    printf("Ingrese la nueva localidad del cliente: ");
    fgets(c->localidad, sizeof(c->localidad), stdin);
    c->localidad[strcspn(c->localidad, "\n")] = '\0'; // Remove newline character
}

void editarPais(Cliente *c) {
    printf("Ingrese el nuevo pais del cliente: ");
    fgets(c->pais, sizeof(c->pais), stdin);
    c->pais[strcspn(c->pais, "\n")] = '\0'; // Remove newline character
}

void editarContrasena(Cliente *c) {
    printf("Ingrese la nueva contrasena del cliente: ");
    fgets(c->contrasena, sizeof(c->contrasena), stdin);
    c->contrasena[strcspn(c->contrasena, "\n")] = '\0'; // Remove newline character
}

void mostrarCliente(Cliente *c) {
    if (c == NULL) {
        printf("El cliente no existe.\n");
        return;
    }
    printf("Nombre: %s\n", c->nombre);
    printf("Apellido: %s\n", c->apellido);
    printf("Edad: %d\n", c->edad);
    printf("CUIT: %ld\n", c->cuit);
    printf("Mail: %s\n", c->mail);
    printf("Telefono: %ld\n", c->telefono);
    printf("Localidad: %s\n", c->localidad);
    printf("Pais: %s\n", c->pais);
}
