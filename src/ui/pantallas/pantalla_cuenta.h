#ifndef PANTALLA_CUENTA_H
#define PANTALLA_CUENTA_H

#include "ventana.h"
#include "cuenta.h"
#include "navegacion.h"

// Pantalla de una cuenta: muestra datos (CBU, alias, saldo) y permite
// ingresar / retirar (formulario desplegable en la misma pantalla) y
// modificar alias. Transferir se delega a NAV (subpantalla aparte) -> ver nota.
//
// Recibe la cuenta por puntero porque las operaciones modifican el saldo.
// Devuelve el siguiente estado de navegacion.
Navegacion pantalla_cuenta(Ventana *v, Cuenta *cuenta);

#endif