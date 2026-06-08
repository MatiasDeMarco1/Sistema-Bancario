#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"

#define ARCHIVO_CONFIG "./config/smtp.conf"

// Quita el salto de linea final si existe
static void quitarSalto(char *s) {
    s[strcspn(s, "\r\n")] = '\0';
}

int cargarConfigSMTP(ConfigSMTP *cfg) {
    cfg->cargado = 0;
    cfg->servidor[0] = '\0';
    cfg->usuario[0]  = '\0';
    cfg->password[0] = '\0';
    cfg->puerto = 465;

    FILE *f = fopen(ARCHIVO_CONFIG, "r");
    if (f == NULL) {
        printf("Aviso: no se encontro %s. El envio de correo esta deshabilitado.\n",
            ARCHIVO_CONFIG);
        return 0;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), f)) {
        quitarSalto(linea);

        // Ignorar comentarios y lineas vacias
        if (linea[0] == '#' || linea[0] == '\0') continue;

        // Separar clave=valor
        char *igual = strchr(linea, '=');
        if (!igual) continue;
        *igual = '\0';
        char *clave = linea;
        char *valor = igual + 1;

        if (strcmp(clave, "smtp_servidor") == 0)
            strncpy(cfg->servidor, valor, sizeof(cfg->servidor) - 1);
        else if (strcmp(clave, "smtp_puerto") == 0)
            cfg->puerto = atoi(valor);
        else if (strcmp(clave, "smtp_usuario") == 0)
            strncpy(cfg->usuario, valor, sizeof(cfg->usuario) - 1);
        else if (strcmp(clave, "smtp_password") == 0)
            strncpy(cfg->password, valor, sizeof(cfg->password) - 1);
    }
    fclose(f);

    // Validar que tenga lo minimo
    if (cfg->servidor[0] && cfg->usuario[0] && cfg->password[0]) {
        cfg->cargado = 1;
        return 1;
    }

    printf("Aviso: %s esta incompleto.\n", ARCHIVO_CONFIG);
    return 0;
}