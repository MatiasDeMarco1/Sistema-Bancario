#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <stdio.h>
#include <stdlib.h>

void validarContrasena(char *buffer, int tamBuffer);
int validar_Cuil(const char *cuil);
int validar_Mail(const char *mail);
int validar_Telefono(const char *tel);

#endif