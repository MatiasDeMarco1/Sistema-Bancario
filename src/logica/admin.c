#include <stdio.h>
#include <string.h>
#include "admin.h"
#include "validaciones.h"   

#define ARCHIVO_ADMIN "./datos/admins.dat"

void guardarAdmin(Admin *a) {
    FILE *f = fopen(ARCHIVO_ADMIN, "ab");
    if (f == NULL) { printf("Error al abrir archivo de admins.\n"); return; }
    fwrite(a, sizeof(Admin), 1, f);
    fclose(f);
}

int buscarAdmin(const char *usuario, Admin *a) {
    FILE *f = fopen(ARCHIVO_ADMIN, "rb");
    if (f == NULL) return 0;
    while (fread(a, sizeof(Admin), 1, f) == 1) {
        if (strcmp(a->usuario, usuario) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// Crea un admin por defecto la primera vez (si no existe el archivo).
// Usuario: admin   Contrasena: admin1234
void seedAdmin(void) {
    FILE *f = fopen(ARCHIVO_ADMIN, "rb");
    if (f != NULL) {           
        fclose(f);
        return;
    }

    Admin a;
    strcpy(a.usuario, "admin");
    char hash[17];
    hashearContrasena("admin1234", hash);  
    strcpy(a.contrasena, hash);

    guardarAdmin(&a);
    printf("Admin inicial creado (usuario: admin, contrasena: admin1234)\n");
}



ResultadoAdmin crearAdmin_op(const char *usuario, const char *contrasena) {
    if (usuario[0] == '\0')                  return ADM_USUARIO_VACIO;

    Admin tmp;
    if (buscarAdmin(usuario, &tmp))          return ADM_USUARIO_DUPLICADO;

    if (!validar_contrasena_str(contrasena)) return ADM_PASS_CORTA;

    Admin nuevo;
    strncpy(nuevo.usuario, usuario, sizeof(nuevo.usuario) - 1);
    nuevo.usuario[sizeof(nuevo.usuario) - 1] = '\0';

    char hash[17];
    hashearContrasena(contrasena, hash);
    strcpy(nuevo.contrasena, hash);

    guardarAdmin(&nuevo);
    return ADM_OK;
}