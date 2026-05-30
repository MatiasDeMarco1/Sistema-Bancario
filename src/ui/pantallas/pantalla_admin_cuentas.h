#ifndef PANTALLA_ADMIN_CUENTAS_H
#define PANTALLA_ADMIN_CUENTAS_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Muestra (solo lectura) las cuentas del cliente. Volver -> NAV_ADMIN_DETALLE.
Navegacion pantalla_admin_cuentas(Ventana *v, Cliente *cliente);

#endif