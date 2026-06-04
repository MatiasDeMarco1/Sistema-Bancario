// Etapa 2: enviar UN mail de prueba aislado, sin tocar el banco.
// Compilar:
//   gcc -Iconfig test_correo.c config/config.c correo/correo.c -o test_correo -lcurl
//   (ajustar rutas segun donde pongas los archivos)
// Uso:
//   ./test_correo destinatario@gmail.com
#include "config.h"
#include "correo.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s destinatario@correo.com\n", argv[0]);
        return 1;
    }

    ConfigSMTP cfg;
    if (!cargarConfigSMTP(&cfg)) {
        printf("No se pudo cargar la config. Revisa config/smtp.conf\n");
        return 1;
    }

    printf("Enviando mail de prueba a %s ...\n", argv[1]);
    int ok = enviarCorreo(&cfg, argv[1],
                          "Prueba Banco UCEL",
                          "Este es un mail de prueba del sistema bancario.");

    if (ok)
        printf("Mail enviado correctamente. Revisa la casilla.\n");
    else
        printf("Fallo el envio.\n");

    return 0;
}