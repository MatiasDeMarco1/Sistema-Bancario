#include "pantalla_login.h"
#include "componentes.h"
#include <string.h>

// De login.c
int login_validar(const char *identificador, const char *contrasena, Cliente *c);

Navegacion pantalla_login(Ventana *v, Cliente *c) {
    int panel_w = 380;
    int panel_h = 400;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int campo_w = 300;
    int campo_x = panel_x + (panel_w - campo_w) / 2;

    Input in_id = input_crear(campo_x, panel_y + 110, campo_w, 42,
                              "CUIT, mail o telefono", 0);
    Input in_pass = input_crear(campo_x, panel_y + 175, campo_w, 42,
                                "Contrasena", 1);

    Boton btn_ingresar = boton_crear(campo_x, panel_y + 245, campo_w, 46,
                                     "INGRESAR", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_registrar = boton_crear(campo_x, panel_y + 305, campo_w, 42,
                                      "Crear cuenta nueva", COLOR_PANEL, COLOR_BORDE);

    char mensaje[80] = "";

    SDL_Event e;
    Navegacion siguiente = NAV_LOGIN;

    while (siguiente == NAV_LOGIN && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            input_manejar_evento(&in_id, &e);
            input_manejar_evento(&in_pass, &e);

            int enter = (e.type == SDL_KEYDOWN &&
                         e.key.keysym.sym == SDLK_RETURN);

            if (boton_fue_clickeado(&btn_ingresar, &e) || enter) {
                if (in_id.largo == 0 || in_pass.largo == 0) {
                    strcpy(mensaje, "Complete todos los campos");
                } else if (login_validar(in_id.texto, in_pass.texto, c)) {
                    if (!c->activo) {
                        strcpy(mensaje, "Tu cuenta esta dada de baja");
                        input_limpiar(&in_pass);
                    } else {
                        siguiente = NAV_MENU;
                    }
                } else {
                    strcpy(mensaje, "Credenciales incorrectas");
                    input_limpiar(&in_pass);
                }
            }

            if (boton_fue_clickeado(&btn_registrar, &e))
                siguiente = NAV_REGISTRO;
        }

        boton_actualizar_hover(&btn_ingresar, mx, my);
        boton_actualizar_hover(&btn_registrar, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        SDL_Rect area_titulo = { panel_x, panel_y + 30, panel_w, 40 };
        texto_centrado(v, v->font_grande, "Banco DMC",
                       area_titulo, COLOR_TEXTO);
        SDL_Rect area_sub = { panel_x, panel_y + 70, panel_w, 24 };
        texto_centrado(v, v->font_chico, "Inicie sesion para continuar",
                       area_sub, COLOR_TEXTO_SUAVE);

        input_dibujar(v, &in_id);
        input_dibujar(v, &in_pass);
        boton_dibujar(v, &btn_ingresar);
        boton_dibujar(v, &btn_registrar);

        if (mensaje[0] != '\0') {
            SDL_Rect area_msg = { panel_x, panel_y + panel_h - 30, panel_w, 24 };
            texto_centrado(v, v->font_chico, mensaje, area_msg, COLOR_PELIGRO);
        }

        ventana_presentar(v);
    }

    return siguiente;
}