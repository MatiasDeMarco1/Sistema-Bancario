#ifndef PANTALLA_SELECCION_CUENTA_H
#define PANTALLA_SELECCION_CUENTA_H

#include "ventana.h"
#include "cliente.h"
#include "cuenta.h"
#include "navegacion.h"

// Muestra las cuentas activas del cliente como tarjetas clickeables.
// Si el usuario elige una, la copia en *cuenta_elegida y devuelve NAV_CUENTA.
// Si toca "Volver", devuelve NAV_MENU.
Navegacion pantalla_seleccion_cuenta(Ventana *v, Cliente *cliente,
                                     Cuenta *cuenta_elegida);

#endif