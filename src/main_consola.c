#include <stdlib.h>
#include "logica/admin.h"
#include "logica/menu.h"

int main(void) {
    system("mkdir -p datos");
    seedAdmin();          // crea el admin inicial si no existe
    menuInicial();
    return 0;
}