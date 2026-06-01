#ifndef PANTALLA_ADMIN_EDITAR_H
#define PANTALLA_ADMIN_EDITAR_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Edicion de datos de un cliente por parte del admin.
// Puede editar nombre, apellido, mail, telefono, localidad, pais.
// (El CUIT no es editable). Volver -> NAV_ADMIN_DETALLE.
Navegacion pantalla_admin_editar(Ventana *v, Cliente *cliente);

#endif