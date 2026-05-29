#include <stdio.h>
#include <string.h>
#include "historial.h"

#define ARCHIVO_MOV "./datos/movimientos.dat"

static void imprimirMovimiento(Movimiento *m, char *cbu_consultado) {
    printf("Moneda: %s\n", m->moneda == PESOS ? "Pesos" : "Dolares");

    switch (m->tipo) {
        case INGRESO:
            printf("Tipo:   Ingreso\n");
            printf("Monto:  +%.2f\n", m->monto);
            break;
        case EGRESO:
            printf("Tipo:   Egreso\n");
            printf("Monto:  -%.2f\n", m->monto);
            break;
        case TRANSFERENCIA:
            if (strcmp(m->cbu_origen, cbu_consultado) == 0) {
                printf("Tipo:   Transferencia enviada\n");
                printf("Para:   %s\n", m->cbu_destino);
                printf("Monto:  -%.2f\n", m->monto);
            } else {
                printf("Tipo:   Transferencia recibida\n");
                printf("De:     %s\n", m->cbu_origen);
                printf("Monto:  +%.2f\n", m->monto);
            }
            break;
    }
    printf("-------------------\n");
}

void mostrarHistorialCuenta(char *cbu) {
    FILE *f = fopen(ARCHIVO_MOV, "rb");
    if (f == NULL) { printf("No hay movimientos registrados.\n"); return; }

    Movimiento m;
    int encontrado = 0;
    printf("\n=== HISTORIAL DE CUENTA %s ===\n", cbu);

    while (fread(&m, sizeof(Movimiento), 1, f)) {
        if (strcmp(m.cbu_origen, cbu) == 0 || strcmp(m.cbu_destino, cbu) == 0) {
            imprimirMovimiento(&m, cbu);
            encontrado = 1;
        }
    }

    if (!encontrado) printf("No hay movimientos para esta cuenta.\n");
    fclose(f);
}

void mostrarHistorialCliente(char *cuit) {
    FILE *f = fopen(ARCHIVO_MOV, "rb");
    if (f == NULL) { printf("No hay movimientos registrados.\n"); return; }

    Movimiento m;
    int encontrado = 0;
    printf("\n=== HISTORIAL DEL CLIENTE %s ===\n", cuit);

    while (fread(&m, sizeof(Movimiento), 1, f)) {
        if (strcmp(m.cuit_origen, cuit) == 0 || strcmp(m.cuit_destino, cuit) == 0) {
            imprimirMovimiento(&m, m.cbu_origen);
            encontrado = 1;
        }
    }

    if (!encontrado) printf("No hay movimientos para este cliente.\n");
    fclose(f);
}