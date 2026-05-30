#ifndef PANTALLA_LOGIN_H
#define PANTALLA_LOGIN_H

#include "ventana.h"
#include "cliente.h"
#include "admin.h"
#include "navegacion.h"

// Pantalla de login unificada. Devuelve:
//   NAV_MENU       -> entro un cliente (cargado en *c)
//   NAV_MENU_ADMIN -> entro un admin (cargado en *a)
//   NAV_REGISTRO   -> el usuario quiere crear una cuenta nueva
//   NAV_SALIR      -> cerro la ventana
Navegacion pantalla_login(Ventana *v, Cliente *c, Admin *a);

#endif