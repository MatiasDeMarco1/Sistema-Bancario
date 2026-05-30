#include <stdlib.h>
#include "logica/admin.h"
#include "ui/app.h"

int main(void) {
    system("mkdir -p datos");
    seedAdmin();          // crea el admin inicial si no existe
    iniciarApp();
    return 0;
}