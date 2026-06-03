#ifndef NAVEGACION_H
#define NAVEGACION_H

// Estados posibles de la aplicacion (que pantalla mostrar).
typedef enum {
    NAV_SALIR,
    NAV_LOGIN,
    NAV_REGISTRO,
    NAV_MENU,
    NAV_PERFIL,
    NAV_SELECCION_CUENTA,
    NAV_CUENTA,
    NAV_TRANSFERIR,
    NAV_HISTORIAL,
    NAV_MENU_ADMIN,
    NAV_ADMIN_CLIENTES,
    NAV_ADMIN_DETALLE,
    NAV_ADMIN_CUENTAS,
    NAV_ADMIN_MOVIMIENTOS,
    NAV_ADMIN_EDITAR,
    NAV_ADMIN_CREAR        // crear un nuevo administrador
} Navegacion;

#endif