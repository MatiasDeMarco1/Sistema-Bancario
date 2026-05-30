#ifndef PANTALLA_MENU_ADMIN_H
#define PANTALLA_MENU_ADMIN_H

#include "ventana.h"
#include "admin.h"
#include "navegacion.h"

// Menu del administrador. Devuelve el siguiente estado de navegacion.
Navegacion pantalla_menu_admin(Ventana *v, Admin *admin);

#endif