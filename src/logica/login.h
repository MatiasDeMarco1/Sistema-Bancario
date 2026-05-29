#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"

int login(Cliente *c);
int login_validar(const char *identificador, const char *contrasena, Cliente *c);