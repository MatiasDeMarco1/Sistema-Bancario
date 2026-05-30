#ifndef PANTALLA_ADMIN_CLIENTES_H
#define PANTALLA_ADMIN_CLIENTES_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Listado de clientes para el admin. Al hacer clic en uno, lo copia en
// *seleccionado y devuelve NAV_ADMIN_DETALLE. "Volver" -> NAV_MENU_ADMIN.
Navegacion pantalla_admin_clientes(Ventana *v, Cliente *seleccionado);

#endif