#include "ventana.h"
#include "navegacion.h"
#include "pantalla_login.h"
#include "pantalla_registro.h"
#include "pantalla_verificar.h"
#include "pantalla_menu.h"
#include "pantalla_perfil.h"
#include "pantalla_seleccion_cuenta.h"
#include "pantalla_cuenta.h"
#include "pantalla_transferir.h"
#include "pantalla_historial.h"
#include "pantalla_menu_admin.h"
#include "pantalla_admin_clientes.h"
#include "pantalla_admin_detalle.h"
#include "pantalla_admin_cuentas.h"
#include "pantalla_admin_movimientos.h"
#include "pantalla_admin_editar.h"
#include "pantalla_admin_crear.h"
#include "cliente.h"
#include "cuenta.h"
#include "admin.h"
#include <stdio.h>

void iniciarApp(void) {
    Ventana v;
    if (!ventana_init(&v)) return;

    Cliente cliente;        // cliente logueado (sesion normal)
    Cuenta  cuenta;         // cuenta seleccionada por el cliente
    Admin   admin;          // admin logueado
    Cliente cliente_admin;  // cliente que el admin esta inspeccionando

    Navegacion actual = NAV_LOGIN;

    while (actual != NAV_SALIR && v.corriendo) {
        switch (actual) {
            case NAV_LOGIN:
                actual = pantalla_login(&v, &cliente, &admin);
                break;
            case NAV_REGISTRO:
                actual = pantalla_registro(&v, &cliente);
                break;
            case NAV_VERIFICAR:
                actual = pantalla_verificar(&v, &cliente);
                break;
            case NAV_MENU:
                actual = pantalla_menu(&v, &cliente);
                break;
            case NAV_PERFIL:
                actual = pantalla_perfil(&v, &cliente);
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
            case NAV_MENU_ADMIN:
                actual = pantalla_menu_admin(&v, &admin);
                break;
            case NAV_ADMIN_CLIENTES:
                actual = pantalla_admin_clientes(&v, &cliente_admin);
                break;
            case NAV_ADMIN_DETALLE:
                actual = pantalla_admin_detalle(&v, &cliente_admin);
                break;
            case NAV_ADMIN_CUENTAS:
                actual = pantalla_admin_cuentas(&v, &cliente_admin);
                break;
            case NAV_ADMIN_MOVIMIENTOS:
                actual = pantalla_admin_movimientos(&v, &cliente_admin);
                break;
            case NAV_ADMIN_EDITAR:
                actual = pantalla_admin_editar(&v, &cliente_admin);
                break;
            case NAV_ADMIN_CREAR:
                actual = pantalla_admin_crear(&v);
                break;
            default:
                actual = NAV_SALIR;
        }
    }

    ventana_destruir(&v);
}