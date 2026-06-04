#include "sesion_smtp.h"

// Definicion de la global declarada extern en el .h
ConfigSMTP g_config_smtp;

void inicializarSMTP(void) {
    cargarConfigSMTP(&g_config_smtp);
}