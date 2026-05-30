// Test temporal de la infraestructura de admin.
// Compilar:
//   gcc -Ilogica test_admin.c logica/admin.c logica/login.c logica/cliente.c \
//       logica/validaciones.c logica/cuenta.c logica/operaciones.c -o test_admin
//   ./test_admin
#include "logica/admin.h"
#include "logica/login.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    system("mkdir -p datos");
    seedAdmin();

    Admin a;
    if (login_admin_validar("admin", "admin1234", &a))
        printf("OK: login admin correcto\n");
    else
        printf("FALLO: deberia haber entrado\n");

    if (!login_admin_validar("admin", "claveMala", &a))
        printf("OK: rechaza clave incorrecta\n");
    else
        printf("FALLO: no deberia entrar\n");

    return 0;
}