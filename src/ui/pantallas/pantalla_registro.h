#ifndef PANTALLA_REGISTRO_H
#define PANTALLA_REGISTRO_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Pantalla de registro de un nuevo cliente.
// Si el registro es exitoso, deja el cliente creado en *registrado
// y devuelve NAV_VERIFICAR (para ingresar el codigo enviado por mail).
// Si cancela, devuelve NAV_LOGIN.
Navegacion pantalla_registro(Ventana *v, Cliente *registrado);

#endif