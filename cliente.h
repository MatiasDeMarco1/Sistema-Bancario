#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char *nombre;
    char *apellido;
    int edad;
    long cuit;
    char *mail;
    long telefono;
    char *localidad;
    char *pais;
    char *contrasena;
} Cliente;

Cliente* crearCliente();

void eliminarCliente(Cliente **c);

void editarCliente(Cliente *c);

void editarNombre(Cliente *c);
void editarApellido(Cliente *c);
void editarEdad(Cliente *c);
void editarCUIT(Cliente *c);
void editarMail(Cliente *c);
void editarTelefono(Cliente *c);
void editarLocalidad(Cliente *c);
void editarPais(Cliente *c);
void editarContrasena(Cliente *c);

void mostrarCliente(Cliente *c);