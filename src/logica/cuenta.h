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
/* int  buscarCuenta(int id, Cuenta *c); */
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

#endif
