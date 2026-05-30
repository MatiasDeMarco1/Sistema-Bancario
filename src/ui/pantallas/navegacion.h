#ifndef NAVEGACION_H
#define NAVEGACION_H

// Estados posibles de la aplicacion (que pantalla mostrar).
// Cada pantalla_*() devuelve uno de estos para indicar a donde ir.
typedef enum {
    NAV_SALIR,            // cerrar la aplicacion
    NAV_LOGIN,            // pantalla de inicio de sesion
    NAV_REGISTRO,         // pantalla de registro de cliente
    NAV_MENU,             // menu post-login (mis cuentas)
    NAV_PERFIL,           // pantalla "mi perfil" (editar datos)
    NAV_SELECCION_CUENTA, // elegir entre cuenta pesos / dolares
    NAV_CUENTA,           // operar sobre una cuenta seleccionada
    NAV_TRANSFERIR,       // subpantalla de transferencia
    NAV_HISTORIAL         // ver historial de movimientos
} Navegacion;

#endif