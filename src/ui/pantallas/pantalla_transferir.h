#ifndef PANTALLA_TRANSFERIR_H
#define PANTALLA_TRANSFERIR_H

#include "ventana.h"
#include "cuenta.h"
#include "navegacion.h"

// Subpantalla de transferencia. Recibe la cuenta origen por puntero
// (se modifica el saldo si la transferencia es exitosa).
// Devuelve NAV_CUENTA al volver.
Navegacion pantalla_transferir(Ventana *v, Cuenta *origen);

#endif