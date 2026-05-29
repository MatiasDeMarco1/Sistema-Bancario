#include "ventana.h"
#include "navegacion.h"
#include "pantalla_login.h"
#include "pantalla_registro.h"
#include "pantalla_menu.h"
#include "pantalla_seleccion_cuenta.h"
#include "pantalla_cuenta.h"
#include "pantalla_transferir.h"
#include "pantalla_historial.h"
#include "cliente.h"
#include "cuenta.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Ventana v;
    if (!ventana_init(&v)) return 1;

    Cliente cliente;
    Cuenta  cuenta;

    Navegacion actual = NAV_LOGIN;

    while (actual != NAV_SALIR && v.corriendo) {
        switch (actual) {

            case NAV_LOGIN:
                actual = pantalla_login(&v, &cliente);
                break;

            case NAV_REGISTRO:
                actual = pantalla_registro(&v);
                break;

            case NAV_MENU:
                actual = pantalla_menu(&v, &cliente);
                break;

            case NAV_SELECCION_CUENTA:
                actual = pantalla_seleccion_cuenta(&v, &cliente, &cuenta);
                break;

            case NAV_CUENTA:
                actual = pantalla_cuenta(&v, &cuenta);
                break;

            case NAV_TRANSFERIR:
                actual = pantalla_transferir(&v, &cuenta);
                break;

            case NAV_HISTORIAL:
                actual = pantalla_historial(&v, &cliente);
                break;

            default:
                actual = NAV_SALIR;
        }
    }

    ventana_destruir(&v);
    printf("Aplicacion cerrada.\n");
    return 0;
}