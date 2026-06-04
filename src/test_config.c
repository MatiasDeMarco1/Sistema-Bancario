// Etapa 1: probar que se lee bien la config.
// Compilar: gcc -Iconfig test_config.c config/config.c -o test_config
// (ajustar rutas segun donde pongas config.c/.h)
#include "config.h"
#include <stdio.h>

int main(void) {
    ConfigSMTP cfg;
    if (cargarConfigSMTP(&cfg)) {
        printf("Config cargada OK:\n");
        printf("  Servidor: %s\n", cfg.servidor);
        printf("  Puerto:   %d\n", cfg.puerto);
        printf("  Usuario:  %s\n", cfg.usuario);
        printf("  Password: %.4s... (oculta)\n", cfg.password);
    } else {
        printf("No se pudo cargar la config.\n");
    }
    return 0;
}