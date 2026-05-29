#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "login.h"

#define ARCHIVO "./datos/clientes.dat"

int login(Cliente *c) {
    char identificador[80];
    char contrasena[64];

    printf("Ingrese CUIT, mail o telefono: ");
    fgets(identificador, sizeof(identificador), stdin);
    identificador[strcspn(identificador, "\n")] = '\0';

    printf("Ingrese contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';

    return login_validar(identificador, contrasena, c);
}

int login_validar(const char *identificador, const char *contrasena, Cliente *c) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) return 0;

    while (fread(c, sizeof(Cliente), 1, f)) {
        if ((strcmp(c->cuit,     identificador) == 0 ||
            strcmp(c->mail,     identificador) == 0 ||
            strcmp(c->telefono, identificador) == 0) &&
            strcmp(c->contrasena, contrasena)  == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}