#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "operaciones.h"


void mostrarHistorialCuenta(char *cbu);
void mostrarHistorialCliente(char *cuit);

// Llena el array 'movs' con los movimientos del cliente (por CUIT).
// 'max' es la capacidad del array. Devuelve cuantos encontro.
int obtenerHistorialCliente(char *cuit, Movimiento movs[], int max);
#endif