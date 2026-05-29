#ifndef PANTALLA_LOGIN_H
#define PANTALLA_LOGIN_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Pantalla de login. Devuelve:
//   NAV_MENU     -> login exitoso (deja el cliente cargado en *c)
//   NAV_REGISTRO -> el usuario quiere crear una cuenta nueva
//   NAV_SALIR    -> cerro la ventana
Navegacion pantalla_login(Ventana *v, Cliente *c);

#endif