#ifndef CUENTA_H
#define CUENTA_H

typedef enum { PESOS, DOLARES } Moneda;

typedef struct {
    int    id;
    char   cliente_cuit[14];
    char   cbu[23];
    char   alias[50];
    Moneda moneda;
    double saldo;
    int    activa;
} Cuenta;

// Archivo
void guardarCuenta(Cuenta *c);
void guardarCambiosCuenta(Cuenta *c);

// ABM
void crearCuenta(Cuenta *c, char *cuit, Moneda moneda);
int  clienteTieneCuenta(char *cuit, Moneda moneda);
void mostrarCuenta(Cuenta *c);
void mostrarCuentasCliente(char *cuit);

// Editar
void editarAlias(Cuenta *c);

// CBU y alias
void generarCBU(char *cbu);
void pedirAlias(char *alias, Moneda moneda);

// Llena el array 'cuentas' con las cuentas activas del cliente.
// 'max' es la capacidad del array. Devuelve cuantas encontro.
int obtenerCuentasCliente(char *cuit, Cuenta cuentas[], int max);

void armarAlias(char *alias, const char *base, Moneda moneda);
void crearCuenta_op(Cuenta *c, char *cuit, Moneda moneda, const char *alias);

int cbuUnico(const char *cbu);
int aliasUnico(const char *alias);
#endif
