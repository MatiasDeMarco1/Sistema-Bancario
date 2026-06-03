#include "pantalla_menu_admin.h"
#include "componentes.h"
#include <stdio.h>

Navegacion pantalla_menu_admin(Ventana *v, Admin *admin) {
    int panel_w = 460;
    int panel_h = 410;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int btn_w = 360;
    int btn_x = panel_x + (panel_w - btn_w) / 2;
    int btn_h = 46;
    int gap   = 14;
    int y0    = panel_y + 140;

    Boton btn_clientes = boton_crear(btn_x, y0, btn_w, btn_h,
        "Gestionar clientes", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_crear_admin = boton_crear(btn_x, y0 + (btn_h + gap), btn_w, btn_h,
        "Crear administrador", COLOR_EXITO, COLOR_EXITO);
    Boton btn_salir = boton_crear(btn_x, y0 + 2 * (btn_h + gap), btn_w, btn_h,
        "Cerrar sesion", COLOR_PANEL, COLOR_BORDE);

    char saludo[120];
    snprintf(saludo, sizeof(saludo), "Administrador: %s", admin->usuario);

    SDL_Event e;
    Navegacion siguiente = NAV_MENU_ADMIN;

    while (siguiente == NAV_MENU_ADMIN && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }
            if (boton_fue_clickeado(&btn_clientes, &e))
                siguiente = NAV_ADMIN_CLIENTES;
            if (boton_fue_clickeado(&btn_crear_admin, &e))
                siguiente = NAV_ADMIN_CREAR;
            if (boton_fue_clickeado(&btn_salir, &e))
                siguiente = NAV_LOGIN;
        }

        boton_actualizar_hover(&btn_clientes, mx, my);
        boton_actualizar_hover(&btn_crear_admin, mx, my);
        boton_actualizar_hover(&btn_salir, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Panel de administracion",
                      panel_x + 40, panel_y + 40, COLOR_TEXTO);
        texto_dibujar(v, v->font_chico, saludo,
                      panel_x + 40, panel_y + 90, COLOR_TEXTO_SUAVE);

        boton_dibujar(v, &btn_clientes);
        boton_dibujar(v, &btn_crear_admin);
        boton_dibujar(v, &btn_salir);

        ventana_presentar(v);
    }

    return siguiente;
}