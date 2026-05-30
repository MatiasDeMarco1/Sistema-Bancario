#ifndef PANTALLA_ADMIN_MOVIMIENTOS_H
#define PANTALLA_ADMIN_MOVIMIENTOS_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Muestra los movimientos del cliente (auditoria). Volver -> NAV_ADMIN_DETALLE.
Navegacion pantalla_admin_movimientos(Ventana *v, Cliente *cliente);

#endif