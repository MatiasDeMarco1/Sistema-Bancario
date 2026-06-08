#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cliente.h"
#include "validaciones.h"
#include <time.h>          
#include "correo.h"        
#include "sesion_smtp.h"   

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


ResultadoRegistro crearCliente_op(Cliente *c) {
    if (c->nombre[0] == '\0' || c->apellido[0] == '\0' ||
        c->localidad[0] == '\0' || c->pais[0] == '\0')
        return REG_CAMPO_VACIO;

    if (!validar_Cuil(c->cuit))              return REG_CUIT_INVALIDO;
    if (validar_cuil_unico(c->cuit) == 0)    return REG_CUIT_DUPLICADO;
    if (!validar_Mail(c->mail))              return REG_MAIL_INVALIDO;
    if (validar_mail_unico(c->mail) == 0)         return REG_MAIL_DUPLICADO;
    if (!validar_Telefono(c->telefono))      return REG_TELEFONO_INVALIDO;
    if (validar_telefono_unico(c->telefono) == 0) return REG_TELEFONO_DUPLICADO;
    if (!validar_contrasena_str(c->contrasena))   return REG_PASS_CORTA;

    // Hashear la contrasena ANTES de guardar (reemplaza el texto plano)
    char hash[17];
    hashearContrasena(c->contrasena, hash);
    strcpy(c->contrasena, hash);

    c->verificado = 0;                  
    generarCodigoVerif(c->codigo_verif);

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

    // Enviar el codigo por mail (si falla, el cliente igual quedo creado)
    enviarCodigoVerif(c);
    return REG_OK;
}


ResultadoEdicion editarMail_op(Cliente *c, const char *nuevo) {
    if (nuevo[0] == '\0')              return EDIT_VACIO;
    if (!validar_Mail(nuevo))          return EDIT_MAIL_INVALIDO;
    // unico, pero permitiendo que sea el mismo que ya tiene
    if (strcmp(nuevo, c->mail) != 0 && validar_mail_unico(nuevo) == 0)
        return EDIT_MAIL_DUPLICADO;
    strcpy(c->mail, nuevo);
    guardarCambios(c);
    return EDIT_OK;
}

ResultadoEdicion editarTelefono_op(Cliente *c, const char *nuevo) {
    if (nuevo[0] == '\0')              return EDIT_VACIO;
    if (!validar_Telefono(nuevo))      return EDIT_TELEFONO_INVALIDO;
    if (strcmp(nuevo, c->telefono) != 0 && validar_telefono_unico(nuevo) == 0)
        return EDIT_TELEFONO_DUPLICADO;
    strcpy(c->telefono, nuevo);
    guardarCambios(c);
    return EDIT_OK;
}

ResultadoEdicion editarLocalidad_op(Cliente *c, const char *nueva) {
    if (nueva[0] == '\0') return EDIT_VACIO;
    strcpy(c->localidad, nueva);
    guardarCambios(c);
    return EDIT_OK;
}

ResultadoEdicion editarPais_op(Cliente *c, const char *nuevo) {
    if (nuevo[0] == '\0') return EDIT_VACIO;
    strcpy(c->pais, nuevo);
    guardarCambios(c);
    return EDIT_OK;
}

ResultadoEdicion editarContrasena_op(Cliente *c, const char *nueva) {
    if (!validar_contrasena_str(nueva)) return EDIT_PASS_CORTA;
    char hash[17];
    hashearContrasena(nueva, hash);     
    strcpy(c->contrasena, hash);
    guardarCambios(c);
    return EDIT_OK;
}

void reactivarCliente(char *cuit) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }
    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f)) {
        if (strcmp(c.cuit, cuit) == 0) {
            fclose(f);
            c.activo = 1;
            guardarCambios(&c);
            return;
        }
    }
    fclose(f);
}

int obtenerTodosLosClientes(Cliente arr[], int max) {
    FILE *f = fopen(ARCHIVO, "rb");
    if (f == NULL) return 0;
    int n = 0;
    while (n < max && fread(&arr[n], sizeof(Cliente), 1, f) == 1) {
        n++;
    }
    fclose(f);
    return n;
}

ResultadoEdicion editarNombre_op(Cliente *c, const char *nuevo) {
    if (nuevo[0] == '\0') return EDIT_VACIO;
    strcpy(c->nombre, nuevo);
    guardarCambios(c);
    return EDIT_OK;
}

ResultadoEdicion editarApellido_op(Cliente *c, const char *nuevo) {
    if (nuevo[0] == '\0') return EDIT_VACIO;
    strcpy(c->apellido, nuevo);
    guardarCambios(c);
    return EDIT_OK;
}

// Genera un codigo de 6 digitos
void generarCodigoVerif(char *cod) {
    for (int i = 0; i < 6; i++)
        cod[i] = '0' + rand() % 10;
    cod[6] = '\0';
}

// Envia el codigo de verificacion al mail del cliente.
// Devuelve 1 si se envio, 0 si fallo (igual no es fatal).
int enviarCodigoVerif(Cliente *c) {
    char asunto[] = "Codigo de verificacion - Banco UCEL";
    char cuerpo[200];
    snprintf(cuerpo, sizeof(cuerpo),
            "Hola %s,\r\nTu codigo de verificacion es: %s\r\n"
            "Ingresalo en la aplicacion para activar tu cuenta.",
            c->nombre, c->codigo_verif);
    return enviarCorreo(&g_config_smtp, c->mail, asunto, cuerpo);
}

// Verifica el codigo ingresado contra el guardado.
// Si coincide, marca al cliente como verificado y guarda. Devuelve 1; si no, 0.
int verificarCodigo(Cliente *c, const char *codigo) {
    if (strcmp(c->codigo_verif, codigo) == 0) {
        c->verificado = 1;
        guardarCambios(c);
        return 1;
    }
    return 0;
}

// Regenera el codigo y lo reenvia por mail. Devuelve 1 si se envio.
int reenviarCodigo(Cliente *c) {
    generarCodigoVerif(c->codigo_verif);
    guardarCambios(c);              // guardar el codigo nuevo
    return enviarCodigoVerif(c);
}