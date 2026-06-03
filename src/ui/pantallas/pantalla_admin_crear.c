#include "pantalla_admin_crear.h"
#include "componentes.h"
#include "admin.h"
#include <string.h>
#include <stdio.h>

// De admin.c
ResultadoAdmin crearAdmin_op(const char *usuario, const char *contrasena);

static const char *mensaje_resultado(ResultadoAdmin r) {
    switch (r) {
        case ADM_OK:               return "Administrador creado con exito";
        case ADM_USUARIO_VACIO:    return "El usuario no puede estar vacio";
        case ADM_USUARIO_DUPLICADO:return "Ese usuario ya existe";
        case ADM_PASS_CORTA:       return "La contrasena debe tener al menos 8 caracteres";
        default:                   return "";
    }
}

Navegacion pantalla_admin_crear(Ventana *v) {
    int panel_w = 440;
    int panel_h = 420;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int campo_w = panel_w - 80;
    int campo_x = panel_x + 40;

    Input in_usuario = input_crear(campo_x, panel_y + 120, campo_w, 42,
                                   "Usuario", 0);
    Input in_pass = input_crear(campo_x, panel_y + 185, campo_w, 42,
                                "Contrasena (min 8)", 1);
    in_usuario.activo = 1;

    Boton btn_crear = boton_crear(campo_x, panel_y + 255, campo_w, 46,
        "Crear administrador", COLOR_EXITO, COLOR_EXITO);
    Boton btn_volver = boton_crear(campo_x, panel_y + 315, campo_w, 42,
        "Volver", COLOR_PANEL, COLOR_BORDE);

    char mensaje[80] = "";
    SDL_Color color_msg = COLOR_PELIGRO;

    SDL_Event e;
    Navegacion siguiente = NAV_ADMIN_CREAR;

    while (siguiente == NAV_ADMIN_CREAR && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            input_manejar_evento(&in_usuario, &e);
            input_manejar_evento(&in_pass, &e);

            // Tab para pasar de usuario a contrasena
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
                if (in_usuario.activo) {
                    in_usuario.activo = 0;
                    in_pass.activo = 1;
                } else {
                    in_pass.activo = 0;
                    in_usuario.activo = 1;
                }
            }

            int enter = (e.type == SDL_KEYDOWN &&
                         e.key.keysym.sym == SDLK_RETURN);

            if (boton_fue_clickeado(&btn_crear, &e) || enter) {
                ResultadoAdmin r = crearAdmin_op(in_usuario.texto, in_pass.texto);
                strcpy(mensaje, mensaje_resultado(r));
                color_msg = (r == ADM_OK) ? COLOR_EXITO : COLOR_PELIGRO;
                if (r == ADM_OK) {
                    input_limpiar(&in_usuario);
                    input_limpiar(&in_pass);
                    in_usuario.activo = 1;
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_MENU_ADMIN;
        }

        boton_actualizar_hover(&btn_crear, mx, my);
        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Nuevo administrador",
                      campo_x, panel_y + 35, COLOR_TEXTO);

        texto_dibujar(v, v->font_chico, "Usuario:",
                      campo_x, panel_y + 98, COLOR_TEXTO_SUAVE);
        input_dibujar(v, &in_usuario);

        texto_dibujar(v, v->font_chico, "Contrasena:",
                      campo_x, panel_y + 163, COLOR_TEXTO_SUAVE);
        input_dibujar(v, &in_pass);

        boton_dibujar(v, &btn_crear);
        boton_dibujar(v, &btn_volver);

        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_chico, mensaje,
                          campo_x, panel_y + panel_h - 28, color_msg);
        }

        ventana_presentar(v);
    }

    return siguiente;
}