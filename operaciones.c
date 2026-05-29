#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "operaciones.h"
#include "cuenta.h"

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
//  LISTAR MOVIMIENTOS
// ================================================================

static void imprimirMovimiento(const Movimiento *m) {
    const char *tipo_str[] = { "INGRESO", "EGRESO", "TRANSFERENCIA" };
    printf("  ID:      %d\n",     m->id);
    printf("  Tipo:    %s\n",     tipo_str[m->tipo]);
    printf("  Moneda:  %s\n",     m->moneda == PESOS ? "Pesos" : "Dolares");
    printf("  Monto:   %.2f\n",   m->monto);
    printf("  Origen:  CBU %s  (CUIT %s)\n", m->cbu_origen,  m->cuit_origen);
    printf("  Destino: CBU %s  (CUIT %s)\n", m->cbu_destino, m->cuit_destino);
    printf("  -------------------------\n");
}

/* Muestra todos los movimientos donde participa el CBU dado. */
void listarMovimientosCuenta(const char *cbu) {
    FILE *f = fopen(ARCHIVO_MOV, "rb");
    if (f == NULL) { printf("No hay movimientos registrados.\n"); return; }

    Movimiento m;
    int encontrado = 0;
    while (fread(&m, sizeof(Movimiento), 1, f)) {
        if (strcmp(m.cbu_origen, cbu) == 0 || strcmp(m.cbu_destino, cbu) == 0) {
            imprimirMovimiento(&m);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay movimientos para esta cuenta.\n");
    fclose(f);
}

/* Muestra todos los movimientos donde participa el CUIT dado. */
void listarMovimientosCliente(const char *cuit) {
    FILE *f = fopen(ARCHIVO_MOV, "rb");
    if (f == NULL) { printf("No hay movimientos registrados.\n"); return; }

    Movimiento m;
    int encontrado = 0;
    while (fread(&m, sizeof(Movimiento), 1, f)) {
        if (strcmp(m.cuit_origen, cuit) == 0 || strcmp(m.cuit_destino, cuit) == 0) {
            imprimirMovimiento(&m);
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay movimientos para este cliente.\n");
    fclose(f);
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
    char cbu_dest[23];
    printf("Ingrese el CBU destino: ");
    fgets(cbu_dest, sizeof(cbu_dest), stdin);
    cbu_dest[strcspn(cbu_dest, "\n")] = '\0';

    /* No se puede transferir a la misma cuenta */
    if (strcmp(cbu_dest, origen->cbu) == 0) {
        printf("No podes transferir a tu propia cuenta.\n");
        return;
    }

    /* Buscar cuenta destino en el archivo */
    FILE *f = fopen("./datos/cuentas.dat", "rb");
    if (f == NULL) { printf("Error al abrir archivo de cuentas.\n"); return; }

    Cuenta destino;
    int encontrada = 0;
    while (fread(&destino, sizeof(Cuenta), 1, f)) {
        if (strcmp(destino.cbu, cbu_dest) == 0 && destino.activa) {
            encontrada = 1;
            break;
        }
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

    /* Actualizar saldos */
    origen->saldo  -= monto;
    destino.saldo  += monto;
    guardarCambiosCuenta(origen);
    guardarCambiosCuenta(&destino);

    /* Registrar movimiento */
    Movimiento m;
    m.id     = proximoIdMovimiento();
    m.tipo   = TRANSFERENCIA;
    m.moneda = origen->moneda;
    m.monto  = monto;
    strcpy(m.cuit_origen,  origen->cliente_cuit);
    strcpy(m.cuit_destino, destino.cliente_cuit);
    strcpy(m.cbu_origen,   origen->cbu);
    strcpy(m.cbu_destino,  destino.cbu);
    guardarMovimiento(&m);

    printf("Transferencia exitosa.\n");
    printf("  Monto:         %.2f\n", monto);
    printf("  Saldo actual:  %.2f\n", origen->saldo);
}