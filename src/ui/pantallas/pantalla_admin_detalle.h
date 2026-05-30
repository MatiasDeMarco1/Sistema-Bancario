#ifndef PANTALLA_ADMIN_DETALLE_H
#define PANTALLA_ADMIN_DETALLE_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Detalle de un cliente para el admin: ver datos y acceder a sus cuentas,
// movimientos, y dar de baja / reactivar.
// Recibe el cliente por puntero (se actualiza el estado activo al dar baja).
Navegacion pantalla_admin_detalle(Ventana *v, Cliente *cliente);

#endif