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
void eliminarCliente(char *c);
/* void editarCliente(Cliente *c); */
/* void mostrarCliente(Cliente *c);
void mostrarTodos(); */

// Editar campos
/* void editarMail(Cliente *c);
void editarTelefono(Cliente *c);
void editarLocalidad(Cliente *c);
void editarPais(Cliente *c);
void editarContrasena(Cliente *c); */

// Archivo
void guardarCliente(Cliente *c);
/* int  buscarCliente(int id, Cliente *c); */
void guardarCambios(Cliente *c);

typedef enum {
    REG_OK,
    REG_CUIT_INVALIDO,
    REG_CUIT_DUPLICADO,
    REG_MAIL_INVALIDO,
    REG_MAIL_DUPLICADO,        
    REG_TELEFONO_INVALIDO,
    REG_TELEFONO_DUPLICADO,    
    REG_PASS_CORTA,
    REG_CAMPO_VACIO
} ResultadoRegistro;

ResultadoRegistro crearCliente_op(Cliente *c);

typedef enum {
    EDIT_OK, EDIT_MAIL_INVALIDO, EDIT_MAIL_DUPLICADO,
    EDIT_TELEFONO_INVALIDO, EDIT_TELEFONO_DUPLICADO,
    EDIT_PASS_CORTA, EDIT_VACIO
} ResultadoEdicion;

ResultadoEdicion editarMail_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarTelefono_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarLocalidad_op(Cliente *c, const char *nueva);
ResultadoEdicion editarPais_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarContrasena_op(Cliente *c, const char *nueva);
#endif