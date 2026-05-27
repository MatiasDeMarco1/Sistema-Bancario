#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "validaciones.h"

#define ARCHIVO "./datos/clientes.dat"

// ---- ARCHIVO ----

void guardarCliente(Cliente *c) {
    FILE *f = fopen(ARCHIVO, "ab");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fwrite(c, sizeof(Cliente), 1, f);
    fclose(f);
}

int buscarCliente(int id, Cliente *c) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return 0; }
    fseek(f, (id - 1) * sizeof(Cliente), SEEK_SET);
    int leido = fread(c, sizeof(Cliente), 1, f);
    fclose(f);
    return leido;
}

void guardarCambios(Cliente *c) {
    FILE *f = fopen(ARCHIVO, "r+b");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fseek(f, (c->id - 1) * sizeof(Cliente), SEEK_SET);
    fwrite(c, sizeof(Cliente), 1, f);
    fclose(f);
}

// ---- ABM ----

void crearCliente(Cliente *c) {
    // Generar id automatico
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) {
        c->id = 1;
    } else {
        fseek(f, 0, SEEK_END);
        c->id = ftell(f) / sizeof(Cliente) + 1;
        fclose(f);
    }
    c->activo = 1;

    printf("Ingrese el nombre del cliente: ");
    fgets(c->nombre, sizeof(c->nombre), stdin);
    c->nombre[strcspn(c->nombre, "\n")] = '\0';

    printf("Ingrese el apellido del cliente: ");
    fgets(c->apellido, sizeof(c->apellido), stdin);
    c->apellido[strcspn(c->apellido, "\n")] = '\0';

    printf("Ingrese el CUIT del cliente (XX-XXXXXXXX-X): ");
    fgets(c->cuit, sizeof(c->cuit), stdin);
    c->cuit[strcspn(c->cuit, "\n")] = '\0';

    printf("Ingrese el mail del cliente: ");
    fgets(c->mail, sizeof(c->mail), stdin);
    c->mail[strcspn(c->mail, "\n")] = '\0';

    printf("Ingrese el telefono del cliente: ");
    fgets(c->telefono, sizeof(c->telefono), stdin);
    c->telefono[strcspn(c->telefono, "\n")] = '\0';

    printf("Ingrese la localidad del cliente: ");
    fgets(c->localidad, sizeof(c->localidad), stdin);
    c->localidad[strcspn(c->localidad, "\n")] = '\0';

    printf("Ingrese el pais del cliente: ");
    fgets(c->pais, sizeof(c->pais), stdin);
    c->pais[strcspn(c->pais, "\n")] = '\0';

    char buffer[100];
    validarContrasena(buffer, sizeof(buffer));
    strcpy(c->contrasena, buffer);

    guardarCliente(c);
}

void eliminarCliente(int id) {
    Cliente c;
    if (!buscarCliente(id, &c)) { printf("Cliente no encontrado.\n"); return; }
    c.activo = 0;
    guardarCambios(&c);
    printf("Cliente eliminado.\n");
}

void editarCliente(Cliente *c) {
    int opcion;
    printf("Seleccione el campo a editar:\n");
    printf("1. Nombre\n");
    printf("2. Apellido\n");
    printf("3. CUIT\n");
    printf("4. Mail\n");
    printf("5. Telefono\n");
    printf("6. Localidad\n");
    printf("7. Pais\n");
    printf("8. Contrasena\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar();

    switch (opcion) {
        case 1: editarNombre(c);     break;
        case 2: editarApellido(c);   break;
        case 3: editarCUIT(c);       break;
        case 4: editarMail(c);       break;
        case 5: editarTelefono(c);   break;
        case 6: editarLocalidad(c);  break;
        case 7: editarPais(c);       break;
        case 8: editarContrasena(c); break;
        default: printf("Opcion invalida.\n"); return;
    }

    guardarCambios(c); // guarda en el archivo despues de editar
}

// ---- EDITAR CAMPOS ----

void editarNombre(Cliente *c) {
    printf("Ingrese el nuevo nombre: ");
    fgets(c->nombre, sizeof(c->nombre), stdin);
    c->nombre[strcspn(c->nombre, "\n")] = '\0';
}

void editarApellido(Cliente *c) {
    printf("Ingrese el nuevo apellido: ");
    fgets(c->apellido, sizeof(c->apellido), stdin);
    c->apellido[strcspn(c->apellido, "\n")] = '\0';
}

void editarCUIT(Cliente *c) {
    printf("Ingrese el nuevo CUIT (XX-XXXXXXXX-X): ");
    fgets(c->cuit, sizeof(c->cuit), stdin);
    c->cuit[strcspn(c->cuit, "\n")] = '\0';
}

void editarMail(Cliente *c) {
    printf("Ingrese el nuevo mail: ");
    fgets(c->mail, sizeof(c->mail), stdin);
    c->mail[strcspn(c->mail, "\n")] = '\0';
}

void editarTelefono(Cliente *c) {
    printf("Ingrese el nuevo telefono: ");
    fgets(c->telefono, sizeof(c->telefono), stdin);
    c->telefono[strcspn(c->telefono, "\n")] = '\0';
}

void editarLocalidad(Cliente *c) {
    printf("Ingrese la nueva localidad: ");
    fgets(c->localidad, sizeof(c->localidad), stdin);
    c->localidad[strcspn(c->localidad, "\n")] = '\0';
}

void editarPais(Cliente *c) {
    printf("Ingrese el nuevo pais: ");
    fgets(c->pais, sizeof(c->pais), stdin);
    c->pais[strcspn(c->pais, "\n")] = '\0';
}

void editarContrasena(Cliente *c) {
    char buffer[100];
    validarContrasena(buffer, sizeof(buffer));
    strcpy(c->contrasena, buffer);
}
// ---- MOSTRAR ----

void mostrarCliente(Cliente *c) {
    if (c == NULL || !c->activo) {
        printf("El cliente no existe.\n");
        return;
    }
    printf("ID:        %d\n",  c->id);
    printf("Nombre:    %s\n",  c->nombre);
    printf("Apellido:  %s\n",  c->apellido);
    printf("CUIT:      %s\n",  c->cuit);
    printf("Mail:      %s\n",  c->mail);
    printf("Telefono:  %s\n",  c->telefono);
    printf("Localidad: %s\n",  c->localidad);
    printf("Pais:      %s\n",  c->pais);
}

void mostrarTodos() {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("No hay clientes registrados.\n"); return; }
    Cliente c;
    int encontrado = 0;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (c.activo) {
            mostrarCliente(&c);
            printf("-------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay clientes activos.\n");
    fclose(f);
}