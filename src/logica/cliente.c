#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "validaciones.h"

#define ARCHIVO "./datos/clientes.dat"

// ---- ARCHIVO ----

/* Recibe la estructura de Cliente como puntero 
FILE -> abre el archivo en en ab -> agregar binario
si no es posible abrir el archivo (se elimino o algun error de algo), devuelve el mensaje de error
Se escribe en lo ultimo, el nuevo cliente.
*/
void guardarCliente(Cliente *c) {
    FILE *f = fopen(ARCHIVO, "ab");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fwrite(c, sizeof(Cliente), 1, f);
    fclose(f);
}


/* int buscarCliente(int id, Cliente *c) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return 0; }
    fseek(f, (id - 1) * sizeof(Cliente), SEEK_SET);
    int leido = fread(c, sizeof(Cliente), 1, f);
    fclose(f);
    return leido;
} */

void guardarCambios(Cliente *c) {
    FILE *f = fopen(ARCHIVO, "r+b");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    fseek(f, (c->id - 1) * sizeof(Cliente), SEEK_SET);
    fwrite(c, sizeof(Cliente), 1, f);
    fclose(f);
}

// ---- ABM ----

/* Recibe como puntero estructura Cliente
    Archivo se abre como rb -> Leer binario
    Si no hay nada en el archivo, id es valor 1 (primer cliente)
    Sino, busca el final del archivo, el id del ultimo cliente cargado y le da el valor id+1
    Se ingresan los datos y se validan en validaciones.c
*/
void crearCliente(Cliente *c) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) {
        c->id = 1;
    } else {
        fseek(f, 0, SEEK_END);
        c->id = ftell(f) / sizeof(Cliente) + 1;
        fclose(f);
    }
    c->activo = 1;

    printf("Ingrese el nombre del cliente: ");
    fgets(c->nombre, sizeof(c->nombre), stdin);
    c->nombre[strcspn(c->nombre, "\n")] = '\0';

    printf("Ingrese el apellido del cliente: ");
    fgets(c->apellido, sizeof(c->apellido), stdin);
    c->apellido[strcspn(c->apellido, "\n")] = '\0';

    // CUIT con validacion
    do {
        printf("Ingrese el CUIT del cliente (XX-XXXXXXXX-X): ");
        fgets(c->cuit, sizeof(c->cuit), stdin);
        c->cuit[strcspn(c->cuit, "\n")] = '\0';
        if (!validar_Cuil(c->cuit))
            printf("Error: CUIT invalido.\n");
        if (!validar_cuil_unico(c-> cuit))
            printf("Ya existe una cuenta con este CUIT. \n");
        
    } while (!validar_Cuil(c->cuit) || !validar_cuil_unico(c->cuit));

    // Mail con validacion
    do {
        printf("Ingrese el mail del cliente: ");
        fgets(c->mail, sizeof(c->mail), stdin);
        c->mail[strcspn(c->mail, "\n")] = '\0';
        if (!validar_Mail(c->mail))
            printf("Error: mail invalido.\n");
    } while (!validar_Mail(c->mail));

    // Telefono con validacion
    do {
        printf("Ingrese el telefono del cliente: ");
        fgets(c->telefono, sizeof(c->telefono), stdin);
        c->telefono[strcspn(c->telefono, "\n")] = '\0';
        if (!validar_Telefono(c->telefono))
            printf("Error: telefono invalido.\n");
    } while (!validar_Telefono(c->telefono));

    printf("Ingrese la localidad del cliente: ");
    fgets(c->localidad, sizeof(c->localidad), stdin);
    c->localidad[strcspn(c->localidad, "\n")] = '\0';

    printf("Ingrese el pais del cliente: ");
    fgets(c->pais, sizeof(c->pais), stdin);
    c->pais[strcspn(c->pais, "\n")] = '\0';

    char buffer[100];
    validarContrasena(buffer, sizeof(buffer));
    strcpy(c->contrasena, buffer);

    guardarCliente(c);
}

/*  Baja logica de cliente -> Activa de 1 a 0
    Se lee archivo en rb -> Leer binario
    Retorna error si el archivo no existe
    Recibe como parametros el CUIT del cliente (Al ser unico)
    CUIT encontrado -> Activo = 0
    Guarda cambios
    Si no encontro Cliente con ese CUIT -> Da el error (como se hace ya logeado, seria un bug que no de un return)
*/
void eliminarCliente(char *cuit) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (strcmp(c.cuit, cuit) == 0) {
            fclose(f);
            c.activo = 0;
            guardarCambios(&c);
            return;
        }
    }
    fclose(f);
    printf("Cliente no encontrado.\n");
}

/* void editarCliente(Cliente *c) {
    int opcion;
    printf("Seleccione el campo a editar:\n");
    printf("1. Mail\n");
    printf("2. Telefono\n");
    printf("3. Localidad\n");
    printf("4. Pais\n");
    printf("5. Contrasena\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar();

    switch (opcion) {
        case 1: editarMail(c);       break;
        case 2: editarTelefono(c);   break;
        case 3: editarLocalidad(c);  break;
        case 4: editarPais(c);       break;
        case 5: editarContrasena(c); break;
        default: printf("Opcion invalida.\n"); return;
    }

    guardarCambios(c); // guarda en el archivo despues de editar
} */

// ---- EDITAR CAMPOS ----



/* void editarMail(Cliente *c) {
    do {
        printf("Ingrese el nuevo mail: ");
        fgets(c->mail, sizeof(c->mail), stdin);
        c->mail[strcspn(c->mail, "\n")] = '\0';
        if (!validar_Mail(c->mail))
            printf("Error: mail invalido.\n");
    } while (!validar_Mail(c->mail));
}

void editarTelefono(Cliente *c) {
    do {
        printf("Ingrese el nuevo telefono: ");
        fgets(c->telefono, sizeof(c->telefono), stdin);
        c->telefono[strcspn(c->telefono, "\n")] = '\0';
        if (!validar_Telefono(c->telefono))
            printf("Error: telefono invalido.\n");
    } while (!validar_Telefono(c->telefono));
}

void editarLocalidad(Cliente *c) {
    printf("Ingrese la nueva localidad: ");
    fgets(c->localidad, sizeof(c->localidad), stdin);
    c->localidad[strcspn(c->localidad, "\n")] = '\0';
}

void editarPais(Cliente *c) {
    printf("Ingrese el nuevo pais: ");
    fgets(c->pais, sizeof(c->pais), stdin);
    c->pais[strcspn(c->pais, "\n")] = '\0';
}

void editarContrasena(Cliente *c) {
    char buffer[100];
    validarContrasena(buffer, sizeof(buffer));
    strcpy(c->contrasena, buffer);
} */
// ---- MOSTRAR ----

/* void mostrarCliente(Cliente *c) {
    if (c == NULL || !c->activo) {
        printf("El cliente no existe.\n");
        return;
    }
    printf("ID:        %d\n",  c->id);
    printf("Nombre:    %s\n",  c->nombre);
    printf("Apellido:  %s\n",  c->apellido);
    printf("CUIT:      %s\n",  c->cuit);
    printf("Mail:      %s\n",  c->mail);
    printf("Telefono:  %s\n",  c->telefono);
    printf("Localidad: %s\n",  c->localidad);
    printf("Pais:      %s\n",  c->pais);
}

void mostrarTodos() {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("No hay clientes registrados.\n"); return; }
    Cliente c;
    int encontrado = 0;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (c.activo) {
            mostrarCliente(&c);
            printf("-------------------\n");
            encontrado = 1;
        }
    }
    if (!encontrado) printf("No hay clientes activos.\n");
    fclose(f);
}


ResultadoRegistro crearCliente_op(Cliente *c) {
    if (c->nombre[0] == '\0' || c->apellido[0] == '\0' ||
        c->localidad[0] == '\0' || c->pais[0] == '\0')
        return REG_CAMPO_VACIO;

    if (!validar_Cuil(c->cuit))              return REG_CUIT_INVALIDO;
    if (validar_cuil_unico(c->cuit) != 1)    return REG_CUIT_DUPLICADO;
    if (!validar_Mail(c->mail))              return REG_MAIL_INVALIDO;
    if (validar_mail_unico(c->mail) != 1)    return REG_MAIL_DUPLICADO;
    if (!validar_Telefono(c->telefono))      return REG_TELEFONO_INVALIDO;
    if (validar_telefono_unico(c->telefono) != 1) return REG_TELEFONO_DUPLICADO;
    if (!validar_contrasena_str(c->contrasena))   return REG_PASS_CORTA;

    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) {
        c->id = 1;
    } else {
        fseek(f, 0, SEEK_END);
        c->id = ftell(f) / sizeof(Cliente) + 1;
        fclose(f);
    }
    c->activo = 1;

    guardarCliente(c);
    return REG_OK;
}
