#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "operaciones.h"
#include "cuenta.h"
#include <time.h>

#define ARCHIVO_MOV "./datos/movimientos.dat"

// ================================================================
//  ARCHIVO
// ================================================================

void guardarMovimiento(Movimiento *m) {
    FILE *f = fopen(ARCHIVO_MOV, "ab");
    if (f == NULL) { printf("Error al abrir archivo de movimientos.\n"); return; }
    fwrite(m, sizeof(Movimiento), 1, f);
    fclose(f);
}

/* Asigna el próximo ID autoincremental leyendo el tamaño del archivo. */
static int proximoIdMovimiento(void) {
    FILE *f = fopen(ARCHIVO_MOV, "rb");
    if (f == NULL) return 1;
    fseek(f, 0, SEEK_END);
    int id = (int)(ftell(f) / sizeof(Movimiento)) + 1;
    fclose(f);
    return id;
}

// ================================================================
//  OPERACIONES
// ================================================================

/*
 * INGRESO: deposita dinero en la cuenta.
 * El CBU/CUIT destino es el mismo que el origen (no hay tercero).
 */
void ingresarDinero(Cuenta *cuenta) {
    double monto;
    printf("Ingrese el monto a depositar: ");
    scanf("%lf", &monto);
    getchar();

    if (monto <= 0) {
        printf("El monto debe ser mayor a 0.\n");
        return;
    }

    cuenta->saldo += monto;
    guardarCambiosCuenta(cuenta);

    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = INGRESO;
    m.moneda = cuenta->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL); 
    strcpy(m.cuit_origen,  cuenta->cliente_cuit);
    strcpy(m.cuit_destino, cuenta->cliente_cuit);
    strcpy(m.cbu_origen,   cuenta->cbu);
    strcpy(m.cbu_destino,  cuenta->cbu);
    guardarMovimiento(&m);

    printf("Deposito exitoso. Saldo actual: %.2f\n", cuenta->saldo);
}

/*
 * EGRESO: retira dinero de la cuenta.
 * El CBU/CUIT destino es el mismo que el origen.
 */
void retirarDinero(Cuenta *cuenta) {
    double monto;
    printf("Ingrese el monto a retirar: ");
    scanf("%lf", &monto);
    getchar();

    if (monto <= 0) {
        printf("El monto debe ser mayor a 0.\n");
        return;
    }
    if (monto > cuenta->saldo) {
        printf("Saldo insuficiente. Saldo actual: %.2f\n", cuenta->saldo);
        return;
    }

    cuenta->saldo -= monto;
    guardarCambiosCuenta(cuenta);

    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = EGRESO;
    m.moneda = cuenta->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL);
    strcpy(m.cuit_origen,  cuenta->cliente_cuit);
    strcpy(m.cuit_destino, cuenta->cliente_cuit);
    strcpy(m.cbu_origen,   cuenta->cbu);
    strcpy(m.cbu_destino,  cuenta->cbu);
    guardarMovimiento(&m);

    printf("Retiro exitoso. Saldo actual: %.2f\n", cuenta->saldo);
}

/*
 * TRANSFERENCIA: mueve dinero de 'origen' a otra cuenta por CBU.
 *   - Busca la cuenta destino en el archivo de cuentas.
 *   - Verifica que exista, esté activa y sea de la misma moneda.
 *   - Registra UN movimiento de tipo TRANSFERENCIA con origen y destino distintos.
 */
void transferir(Cuenta *origen, const char *cuit_titular) {
    int metodo;
    printf("Transferir por:\n");
    printf("1. CBU\n");
    printf("2. Alias\n");
    printf("Opcion: ");
    scanf("%d", &metodo);
    getchar();

    if (metodo != 1 && metodo != 2) {
        printf("Opcion invalida.\n");
        return;
    }

    char busqueda[50];
    if (metodo == 1) {
        printf("Ingrese el CBU destino: ");
        fgets(busqueda, 23, stdin);
    } else {
        printf("Ingrese el alias destino: ");
        fgets(busqueda, 50, stdin);
    }
    busqueda[strcspn(busqueda, "\n")] = '\0';

    /* --- No se puede transferir a la misma cuenta --- */
    if (metodo == 1 && strcmp(busqueda, origen->cbu)   == 0) {
        printf("No podes transferir a tu propia cuenta.\n");
        return;
    }
    if (metodo == 2 && strcmp(busqueda, origen->alias) == 0) {
        printf("No podes transferir a tu propia cuenta.\n");
        return;
    }

    /* --- Buscar cuenta destino en el archivo --- */
    FILE *f = fopen("./datos/cuentas.dat", "rb");
    if (f == NULL) { printf("Error al abrir archivo de cuentas.\n"); return; }

    Cuenta destino;
    int encontrada = 0;
    while (fread(&destino, sizeof(Cuenta), 1, f)) {
        if (!destino.activa) continue;
        if (metodo == 1 && strcmp(destino.cbu,   busqueda) == 0) { encontrada = 1; break; }
        if (metodo == 2 && strcmp(destino.alias, busqueda) == 0) { encontrada = 1; break; }
    }
    fclose(f);

    if (!encontrada) {
        printf("Cuenta destino no encontrada o inactiva.\n");
        return;
    }
    if (destino.moneda != origen->moneda) {
        printf("No se puede transferir entre cuentas de distinta moneda.\n");
        return;
    }

    /* --- Monto --- */
    double monto;
    printf("Ingrese el monto a transferir: ");
    scanf("%lf", &monto);
    getchar();

    if (monto <= 0) {
        printf("El monto debe ser mayor a 0.\n");
        return;
    }
    if (monto > origen->saldo) {
        printf("Saldo insuficiente. Saldo actual: %.2f\n", origen->saldo);
        return;
    }

    /* --- Actualizar saldos --- */
    origen->saldo -= monto;
    destino.saldo += monto;
    guardarCambiosCuenta(origen);
    guardarCambiosCuenta(&destino);

    /* --- Registrar movimiento --- */
    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = TRANSFERENCIA;
    m.moneda = origen->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL);
    strcpy(m.cuit_origen,  origen->cliente_cuit);
    strcpy(m.cuit_destino, destino.cliente_cuit);
    strcpy(m.cbu_origen,   origen->cbu);
    strcpy(m.cbu_destino,  destino.cbu);
    guardarMovimiento(&m);

    printf("Transferencia exitosa.\n");
    printf("  Monto:        %.2f\n", monto);
    printf("  Saldo actual: %.2f\n", origen->saldo);
}

// Ingreso: el monto ya viene capturado desde la GUI
ResultadoOp ingresarDinero_op(Cuenta *cuenta, double monto) {
    if (monto <= 0) return OP_MONTO_INVALIDO;

    cuenta->saldo += monto;
    guardarCambiosCuenta(cuenta);

    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = INGRESO;
    m.moneda = cuenta->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL);
    strcpy(m.cuit_origen,  cuenta->cliente_cuit);
    strcpy(m.cuit_destino, cuenta->cliente_cuit);
    strcpy(m.cbu_origen,   cuenta->cbu);
    strcpy(m.cbu_destino,  cuenta->cbu);
    guardarMovimiento(&m);

    return OP_OK;
}

ResultadoOp retirarDinero_op(Cuenta *cuenta, double monto) {
    if (monto <= 0)            return OP_MONTO_INVALIDO;
    if (monto > cuenta->saldo) return OP_SALDO_INSUFICIENTE;

    cuenta->saldo -= monto;
    guardarCambiosCuenta(cuenta);

    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = EGRESO;
    m.moneda = cuenta->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL);
    strcpy(m.cuit_origen,  cuenta->cliente_cuit);
    strcpy(m.cuit_destino, cuenta->cliente_cuit);
    strcpy(m.cbu_origen,   cuenta->cbu);
    strcpy(m.cbu_destino,  cuenta->cbu);
    guardarMovimiento(&m);

    return OP_OK;
}

// Transferencia: recibe metodo (1=CBU, 2=alias), el dato de busqueda y el monto
ResultadoOp transferir_op(Cuenta *origen, int metodo,
                          const char *busqueda, double monto) {
    // No transferir a la propia cuenta
    if (metodo == 1 && strcmp(busqueda, origen->cbu)   == 0) return OP_MISMA_CUENTA;
    if (metodo == 2 && strcmp(busqueda, origen->alias) == 0) return OP_MISMA_CUENTA;

    // Buscar cuenta destino
    FILE *f = fopen("./datos/cuentas.dat", "rb");
    if (f == NULL) return OP_ERROR_ARCHIVO;

    Cuenta destino;
    int encontrada = 0;
    while (fread(&destino, sizeof(Cuenta), 1, f)) {
        if (!destino.activa) continue;
        if (metodo == 1 && strcmp(destino.cbu,   busqueda) == 0) { encontrada = 1; break; }
        if (metodo == 2 && strcmp(destino.alias, busqueda) == 0) { encontrada = 1; break; }
    }
    fclose(f);

    if (!encontrada)                       return OP_DESTINO_NO_ENCONTRADO;
    if (destino.moneda != origen->moneda)  return OP_DISTINTA_MONEDA;
    if (monto <= 0)                        return OP_MONTO_INVALIDO;
    if (monto > origen->saldo)             return OP_SALDO_INSUFICIENTE;

    // Actualizar saldos
    origen->saldo  -= monto;
    destino.saldo  += monto;
    guardarCambiosCuenta(origen);
    guardarCambiosCuenta(&destino);

    // Registrar movimiento
    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = TRANSFERENCIA;
    m.moneda = origen->moneda;
    m.monto  = monto;
    m.fecha  = time(NULL);
    strcpy(m.cuit_origen,  origen->cliente_cuit);
    strcpy(m.cuit_destino, destino.cliente_cuit);
    strcpy(m.cbu_origen,   origen->cbu);
    strcpy(m.cbu_destino,  destino.cbu);
    guardarMovimiento(&m);

    return OP_OK;
}