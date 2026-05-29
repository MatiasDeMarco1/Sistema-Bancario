#ifndef MENU_H
#define MENU_H

#include "cliente.h"
#include "cuenta.h"

void menuInicial();
void menuPostLogin(Cliente *cliente);
void menuSeleccionCuenta(Cliente *cliente);
void menuCuenta(Cuenta *cuenta);

#endif