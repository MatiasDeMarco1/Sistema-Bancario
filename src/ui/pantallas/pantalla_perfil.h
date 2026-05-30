#ifndef PANTALLA_PERFIL_H
#define PANTALLA_PERFIL_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Pantalla "Mi perfil": muestra los datos del cliente y permite editar
// mail, telefono, localidad, pais y contrasena. Devuelve NAV_MENU al volver.
// Recibe el cliente por puntero (se actualiza al editar).
Navegacion pantalla_perfil(Ventana *v, Cliente *cliente);

#endif