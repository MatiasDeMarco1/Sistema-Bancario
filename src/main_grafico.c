#include <stdlib.h>
#include <time.h>
#include "logica/admin.h"
#include "logica/sesion_smtp.h"
#include "ui/app.h"

int main(void) {
    system("mkdir -p datos");
    srand(time(NULL));        // sembrar rand una sola vez (codigos, CBU)
    inicializarSMTP();        // cargar config SMTP una vez
    seedAdmin();
    iniciarApp();
    return 0;
}