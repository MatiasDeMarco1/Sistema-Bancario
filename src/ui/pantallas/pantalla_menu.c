#include "pantalla_menu.h"
#include "componentes.h"
#include "cuenta.h"
#include <string.h>
#include <stdio.h>

// De cuenta.c / cuenta.h: indica si el cliente ya tiene cuenta en esa moneda
int clienteTieneCuenta(char *cuit, Moneda moneda);

Navegacion pantalla_menu(Ventana *v, Cliente *cliente) {
    // Consultar que cuentas tiene (igual que en menuPostLogin)
    int tienePesos   = clienteTieneCuenta(cliente->cuit, PESOS);
    int tieneDolares = clienteTieneCuenta(cliente->cuit, DOLARES);
    int tieneAlguna  = (tienePesos || tieneDolares);

    // --- Layout ---
    int panel_w = 460;
    int panel_h = 420;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int btn_w = 360;
    int btn_x = panel_x + (panel_w - btn_w) / 2;
    int btn_h = 46;
    int gap   = 14;
    int y0    = panel_y + 140;   // primer boton

    // Botones de navegacion
    Boton btn_seleccionar = boton_crear(btn_x, y0, btn_w, btn_h,
        "Seleccionar cuenta", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);

    Boton btn_historial = boton_crear(btn_x, y0 + (btn_h + gap), btn_w, btn_h,
        "Ver historial", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);

    Boton btn_baja = boton_crear(btn_x, y0 + 2 * (btn_h + gap), btn_w, btn_h,
        "Darme de baja", COLOR_PELIGRO, COLOR_PELIGRO);

    Boton btn_salir = boton_crear(btn_x, y0 + 3 * (btn_h + gap), btn_w, btn_h,
        "Cerrar sesion", COLOR_PANEL, COLOR_BORDE);

    // Texto dinamico para el saludo y el subtitulo de cuentas
    char saludo[120];
    snprintf(saludo, sizeof(saludo), "Hola, %s %s",
             cliente->nombre, cliente->apellido);

    char estado_cuentas[120];
    if (tieneAlguna) {
        snprintf(estado_cuentas, sizeof(estado_cuentas), "Cuentas activas: %s%s%s",
                 tienePesos ? "Pesos" : "",
                 (tienePesos && tieneDolares) ? " y " : "",
                 tieneDolares ? "Dolares" : "");
    } else {
        strcpy(estado_cuentas, "Todavia no tenes cuentas activas");
    }

    SDL_Event e;
    Navegacion siguiente = NAV_MENU;   // se mantiene hasta que el usuario elija

    while (siguiente == NAV_MENU && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            // Solo permitir "seleccionar cuenta" / "historial" si tiene cuentas
            if (tieneAlguna && boton_fue_clickeado(&btn_seleccionar, &e))
                siguiente = NAV_SELECCION_CUENTA;

            if (tieneAlguna && boton_fue_clickeado(&btn_historial, &e))
                siguiente = NAV_HISTORIAL;

            if (boton_fue_clickeado(&btn_baja, &e)) {
                // La baja real (eliminarCliente) la conectamos cuando armemos
                // el dialogo de confirmacion. Por ahora vuelve al login.
                siguiente = NAV_LOGIN;
            }

            if (boton_fue_clickeado(&btn_salir, &e))
                siguiente = NAV_LOGIN;
        }

        // Hover (solo en los botones activos)
        if (tieneAlguna) {
            boton_actualizar_hover(&btn_seleccionar, mx, my);
            boton_actualizar_hover(&btn_historial, mx, my);
        }
        boton_actualizar_hover(&btn_baja, mx, my);
        boton_actualizar_hover(&btn_salir, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        // Saludo y estado de cuentas
        texto_dibujar(v, v->font_grande, saludo,
                      panel_x + 40, panel_y + 40, COLOR_TEXTO);
        texto_dibujar(v, v->font_chico, estado_cuentas,
                      panel_x + 40, panel_y + 90, COLOR_TEXTO_SUAVE);

        // Botones: los de cuentas se ven "apagados" si no tiene cuentas
        if (tieneAlguna) {
            boton_dibujar(v, &btn_seleccionar);
            boton_dibujar(v, &btn_historial);
        } else {
            // Dibujar en gris para indicar que estan deshabilitados
            rect_relleno(v, btn_seleccionar.rect, COLOR_BORDE);
            texto_centrado(v, v->font_normal, btn_seleccionar.texto,
                           btn_seleccionar.rect, COLOR_TEXTO_SUAVE);
            rect_relleno(v, btn_historial.rect, COLOR_BORDE);
            texto_centrado(v, v->font_normal, btn_historial.texto,
                           btn_historial.rect, COLOR_TEXTO_SUAVE);
        }

        boton_dibujar(v, &btn_baja);
        boton_dibujar(v, &btn_salir);

        ventana_presentar(v);
    }

    return siguiente;
}