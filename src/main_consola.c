#include <stdlib.h>
#include <time.h>
#include "logica/admin.h"
#include "logica/sesion_smtp.h"
#include "logica/menu.h"

int main(void) {
    system("mkdir -p datos");
    srand(time(NULL));
    inicializarSMTP();
    seedAdmin();
    menuInicial();
    return 0;
}