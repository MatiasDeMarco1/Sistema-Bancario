#include <stdio.h>
#include "cliente.h"    
#include "cuenta.h"
#include <stdlib.h>



int main(){
    Cliente *cliente1 = crearCliente(cliente1);
    if (cliente1 == NULL) {
        return 1; // Salir si no se pudo crear el cliente
    }
    mostrarCliente(cliente1);
    editarCliente(cliente1);
    mostrarCliente(cliente1);
    eliminarCliente(&cliente1);
    return 0;
} 