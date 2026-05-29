#ifndef OPERACIONES_H
#define OPERACIONES_H

#include "cuenta.h"

// ---- TIPOS ----

typedef enum {
    INGRESO,
    EGRESO,
    TRANSFERENCIA
} TipoMovimiento;

typedef struct {
    int            id;
    TipoMovimiento tipo;
    Moneda         moneda;
    char           cuit_origen[14];
    char           cuit_destino[14];  // igual a origen si es ingreso/egreso
    char           cbu_origen[23];
    char           cbu_destino[23];   // igual a origen si es ingreso/egreso
    double         monto;             // siempre positivo; tipo indica sentido
} Movimiento;

// ---- ARCHIVO ----

void guardarMovimiento(Movimiento *m);

// ---- OPERACIONES ----

void ingresarDinero(Cuenta *cuenta);
void retirarDinero(Cuenta *cuenta);
void transferir(Cuenta *origen, const char *cuit_titular);

typedef enum {
    OP_OK,
    OP_MONTO_INVALIDO,
    OP_SALDO_INSUFICIENTE,
    OP_DESTINO_NO_ENCONTRADO,
    OP_MISMA_CUENTA,
    OP_DISTINTA_MONEDA,
    OP_ERROR_ARCHIVO
} ResultadoOp;

ResultadoOp ingresarDinero_op(Cuenta *cuenta, double monto);
ResultadoOp retirarDinero_op(Cuenta *cuenta, double monto);
ResultadoOp transferir_op(Cuenta *origen, int metodo,
                          const char *busqueda, double monto);
#endif