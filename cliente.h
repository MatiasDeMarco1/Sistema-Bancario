#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int  id;
    char nombre[50];
    char apellido[50];
    char cuit[14];
    char mail[80];
    char telefono[20];
    char localidad[50];
    char pais[30];
    char contrasena[64];
    int  activo;
} Cliente;

// ABM
void crearCliente(Cliente *c);
void eliminarCliente(int id);
void editarCliente(Cliente *c);
void mostrarCliente(Cliente *c);
void mostrarTodos();

// Editar campos
void editarNombre(Cliente *c);
void editarApellido(Cliente *c);
void editarCUIT(Cliente *c);
void editarMail(Cliente *c);
void editarTelefono(Cliente *c);
void editarLocalidad(Cliente *c);
void editarPais(Cliente *c);
void editarContrasena(Cliente *c);

// Validaciones
void validarContrasena(char *buffer, int tamBuffer);

// Archivo
void guardarCliente(Cliente *c);
int  buscarCliente(int id, Cliente *c);
void guardarCambios(Cliente *c);

#endif