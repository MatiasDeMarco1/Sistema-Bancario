#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    char servidor[100];
    int  puerto;
    char usuario[100];
    char password[100];
    int  cargado;      // 1 si se leyo bien el archivo, 0 si no
} ConfigSMTP;

// Lee config/smtp.conf y carga las credenciales en *cfg.
// Devuelve 1 si lo logro, 0 si el archivo no existe o esta mal.
int cargarConfigSMTP(ConfigSMTP *cfg);

#endif