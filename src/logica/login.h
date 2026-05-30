#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "admin.h"

int login(Cliente *c);
int login_validar(const char *identificador, const char *contrasena, Cliente *c);

int login_admin_validar(const char *usuario, const char *contrasena, Admin *a);