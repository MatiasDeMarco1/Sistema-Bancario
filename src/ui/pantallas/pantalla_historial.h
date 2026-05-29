#ifndef PANTALLA_HISTORIAL_H
#define PANTALLA_HISTORIAL_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Muestra el historial de movimientos del cliente (lista con scroll).
// Devuelve NAV_MENU al volver.
Navegacion pantalla_historial(Ventana *v, Cliente *cliente);

#endif