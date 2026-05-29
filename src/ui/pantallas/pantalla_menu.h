#ifndef PANTALLA_MENU_H
#define PANTALLA_MENU_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Menu post-login. Muestra las cuentas del cliente y las acciones
// disponibles. Devuelve el siguiente estado de navegacion.
//
// Puede crear cuentas (en pesos/dolares) usando la logica existente,
// porque crearCuenta pide el alias por consola -> ver nota en el .c
Navegacion pantalla_menu(Ventana *v, Cliente *cliente);

#endif