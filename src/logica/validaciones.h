#ifndef VALIDACIONES_H
#define VALIDACIONES_H
#include <stdio.h>
#include <stdlib.h>

void validarContrasena(char *buffer, int tamBuffer);
int validar_Cuil(const char *cuil);
int validar_Mail(const char *mail);
int validar_Telefono(const char *tel);
int validar_cuil_unico(const char *cuil);

int validar_contrasena_str(const char *pass);
int validar_mail_unico(const char *mail);
int validar_telefono_unico(const char *tel);

void hashearContrasena(const char *pass, char *salida);
#endif