#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cuenta.h"
#include "validaciones.h"

#define ARCHIVO_CUENTAS "./datos/cuentas.dat"

// ---- ARCHIVO ----

void guardarCuenta(Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "ab");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fwrite(c, sizeof(Cuenta), 1, f);
    fclose(f);
}

int buscarCuenta(int id, Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return 0; }
    fseek(f, (id - 1) * sizeof(Cuenta), SEEK_SET);
    int leido = fread(c, sizeof(Cuenta), 1, f);
    fclose(f);
    return leido;
}

void guardarCambiosCuenta(Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "r+b");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fseek(f, (c->id - 1) * sizeof(Cuenta), SEEK_SET);
    fwrite(c, sizeof(Cuenta), 1, f);
    fclose(f);
}

// ---- CBU Y ALIAS ----

void generarCBU(char *cbu) {
    srand(time(NULL));
    for (int i = 0; i < 22; i++)
        cbu[i] = '0' + rand() % 10;
    cbu[22] = '\0';
}

void pedirAlias(char *alias, Moneda moneda) {
    char base[40];
    const char *sufijo = (moneda == PESOS) ? ".ars" : ".usd";

    do {
        printf("Ingrese un alias (se agrega %s automaticamente): ", sufijo);
        fgets(base, sizeof(base), stdin);
        base[strcspn(base, "\n")] = '\0';
        if (strlen(base) == 0)
            printf("Error: el alias no puede estar vacio.\n");
    } while (strlen(base) == 0);

    snprintf(alias, 50, "%s%s", base, sufijo);
}

// ---- ABM ----

int clienteTieneCuenta(char *cuit, Moneda moneda) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) return 0;
    Cuenta c;
    while (fread(&c, sizeof(Cuenta), 1, f)) {
        if (strcmp(c.cliente_cuit, cuit) == 0 && c.moneda == moneda && c.activa) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void crearCuenta(Cuenta *c, char *cuit, Moneda moneda) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) {
        c->id = 1;
    } else {
        fseek(f, 0, SEEK_END);
        c->id = ftell(f) / sizeof(Cuenta) + 1;
        fclose(f);
    }

    strcpy(c->cliente_cuit, cuit);
    c->moneda = moneda;
    c->saldo  = 0.0;
    c->activa = 1;

    generarCBU(c->cbu);
    pedirAlias(c->alias, moneda);

    guardarCuenta(c);

    printf("\nCuenta creada exitosamente.\n");
    printf("CBU:   %s\n", c->cbu);
    printf("Alias: %s\n", c->alias);
}

void mostrarCuenta(Cuenta *c) {
    printf("ID:     %d\n",   c->id);
    printf("CBU:    %s\n",   c->cbu);
    printf("Alias:  %s\n",   c->alias);
    printf("Moneda: %s\n",   c->moneda == PESOS ? "Pesos" : "Dolares");
    printf("Saldo:  %.2f\n", c->saldo);
}

void mostrarCuentasCliente(char *cuit) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) { printf("No hay cuentas registradas.\n"); return; }
    Cuenta c;
    int encontrado = 0;
    while (fread(&c, sizeof(Cuenta), 1, f)) {
        if (strcmp(c.cliente_cuit, cuit) == 0 && c.activa) {
            mostrarCuenta(&c);
            printf("-------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No tiene cuentas activas.\n");
    fclose(f);
}

// ---- EDITAR ----

void editarAlias(Cuenta *c) {
    pedirAlias(c->alias, c->moneda); // reutiliza pedirAlias que ya agrega el sufijo
    guardarCambiosCuenta(c);
    printf("Alias actualizado: %s\n", c->alias);
}

int obtenerCuentasCliente(char *cuit, Cuenta cuentas[], int max) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) return 0;

    Cuenta c;
    int n = 0;
    while (n < max && fread(&c, sizeof(Cuenta), 1, f)) {
        if (strcmp(c.cliente_cuit, cuit) == 0 && c.activa) {
            cuentas[n] = c;
            n++;
        }
    }
    fclose(f);
    return n;
}

// Arma el alias final agregando el sufijo segun la moneda.
// 'base' es lo que escribio el usuario (sin sufijo).
void armarAlias(char *alias, const char *base, Moneda moneda) {
    const char *sufijo = (moneda == PESOS) ? ".ars" : ".usd";
    snprintf(alias, 50, "%s%s", base, sufijo);
}

void crearCuenta_op(Cuenta *c, char *cuit, Moneda moneda, const char *alias) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) {
        c->id = 1;
    } else {
        fseek(f, 0, SEEK_END);
        c->id = ftell(f) / sizeof(Cuenta) + 1;
        fclose(f);
    }

    strcpy(c->cliente_cuit, cuit);
    c->moneda = moneda;
    c->saldo  = 0.0;
    c->activa = 1;

    // Generar CBU hasta que sea unico
    do {
        generarCBU(c->cbu);
    } while (!cbuUnico(c->cbu));

    strcpy(c->alias, alias);   // alias ya viene validado y armado

    guardarCuenta(c);
}

int cbuUnico(const char *cbu) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) return 1;   // no hay cuentas, es unico
    Cuenta c;
    while (fread(&c, sizeof(Cuenta), 1, f)) {
        if (strcmp(c.cbu, cbu) == 0) {   // el CBU es unico aunque la cuenta este inactiva
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}

int aliasUnico(const char *alias) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) return 1;
    Cuenta c;
    while (fread(&c, sizeof(Cuenta), 1, f)) {
        if (c.activa && strcmp(c.alias, alias) == 0) {
            fclose(f);
            return 0;
        }
    }
    fclose(f);
    return 1;
}