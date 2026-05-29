#ifndef PANTALLA_REGISTRO_H
#define PANTALLA_REGISTRO_H

#include "ventana.h"
#include "navegacion.h"

// Pantalla de registro de un nuevo cliente.
// Crea solo el cliente (la cuenta se crea luego desde el menu).
// Devuelve NAV_LOGIN al volver (registrado o cancelado).
Navegacion pantalla_registro(Ventana *v);

#endif