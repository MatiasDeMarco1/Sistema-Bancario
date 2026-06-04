#ifndef PANTALLA_VERIFICAR_H
#define PANTALLA_VERIFICAR_H

#include "ventana.h"
#include "cliente.h"
#include "navegacion.h"

// Pantalla para ingresar el codigo de verificacion enviado por mail.
// Recibe el cliente a verificar (recien registrado).
// Si verifica OK -> NAV_LOGIN (ya puede iniciar sesion).
// "Volver" -> NAV_LOGIN tambien (queda sin verificar).
Navegacion pantalla_verificar(Ventana *v, Cliente *cliente);

#endif