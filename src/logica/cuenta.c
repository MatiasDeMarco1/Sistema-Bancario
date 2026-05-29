#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "cuenta.h"
#include "validaciones.h"

#define ARCHIVO_CUENTAS "./datos/cuentas.dat"

// ---- ARCHIVO ----

/*  Abre archivo en forma ab -> Agregar binario
    Si archivo no existe, muestra mensaje de error
    Recibe puntero de estructura Cuenta
    Guarda al final del archivo la nueva cuenta
*/
void guardarCuenta(Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "ab");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fwrite(c, sizeof(Cuenta), 1, f);
    fclose(f);
}

/*  Abre archivo en forma rb -> Leer binario
    Recibe id y puntero de estructura Cuenta
    Mensaje de error si archivo no existe
    Busca en el archivo, el id de la cuenta, retorna si la encontro.
*/

/* int buscarCuenta(int id, Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return 0; }
    fseek(f, (id - 1) * sizeof(Cuenta), SEEK_SET);
    int leido = fread(c, sizeof(Cuenta), 1, f);
    fclose(f);
    return leido;
} */


/*  Recibe puntero de estructura Cuenta
    Se abre archivo en r+b -> Permite leer y escribir binario
    Da error si archivo no existe
    guarda los cambios en el archivo, no elimina, solo MODIFICA
*/

void guardarCambiosCuenta(Cuenta *c) {
    FILE *f = fopen(ARCHIVO_CUENTAS, "r+b");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fseek(f, (c->id - 1) * sizeof(Cuenta), SEEK_SET);
    fwrite(c, sizeof(Cuenta), 1, f);
    fclose(f);
}

// ---- CBU Y ALIAS ----

/*  Genera un CBU randon con una cantidad limitada de caracteres
    Recibe como puntero el valor CBU tipo CHAR
*/
void generarCBU(char *cbu) {
    srand(time(NULL));
    for (int i = 0; i < 22; i++)
        cbu[i] = '0' + rand() % 10;
    cbu[22] = '\0';
}

/*  Se ingresa el alias que quiere el cliente, lo recibe como puntero
    Recibe el enum MONEDA (PESOS - DOLARES)
    El tipo de moneda sirve para saber que tipo de cuenta cada alias y su sufijo
    .ars -> Pesos (Inflacion)
    .usd -> Dolares ($$$$$$$$$$$)
    Si se quiere hacer un alias vacio -> "", retorna error...
    Guarda el alias en la cuenta
*/
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

/*
*/
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