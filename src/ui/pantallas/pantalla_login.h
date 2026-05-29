#ifndef PANTALLA_LOGIN_H
#define PANTALLA_LOGIN_H

#include "ventana.h"
#include "cliente.h"

// Muestra la pantalla de login y maneja el loop hasta que:
//   - el usuario inicia sesion correctamente -> devuelve 1
//     (deja los datos del cliente cargados en *c)
//   - el usuario cierra la ventana          -> devuelve 0
//
// Recibe el Cliente por puntero para cargarlo si el login es exitoso.
int pantalla_login(Ventana *v, Cliente *c);

#endif