#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int cbu;
    char *alias;
    char *nombre;
    char *apellido;
    long telefono;
    char *mail;
    long cuit;
} Cuenta;

Cuenta* crearCuenta();

void eliminarCuenta(Cuenta **cuenta);

void mostrarCuenta(Cuenta *cuenta);

void editarCuenta(Cuenta *cuenta);

void editarAlias(Cuenta *cuenta); 
void editarNombre(Cuenta *cuenta);
void editarApellido(Cuenta *cuenta);
void editarTelefono(Cuenta *cuenta);
void editarMail(Cuenta *cuenta);
