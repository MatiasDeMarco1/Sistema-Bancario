#include "pantalla_verificar.h"
#include "componentes.h"
#include <string.h>
#include <stdio.h>

// De cliente.c
int verificarCodigo(Cliente *c, const char *codigo);
int reenviarCodigo(Cliente *c);

Navegacion pantalla_verificar(Ventana *v, Cliente *cliente) {
    int panel_w = 440;
    int panel_h = 400;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int campo_w = panel_w - 80;
    int campo_x = panel_x + 40;

    Input in_codigo = input_crear(campo_x, panel_y + 150, campo_w, 44,
                                  "Codigo de 6 digitos", 0);
    in_codigo.activo = 1;

    Boton btn_verificar = boton_crear(campo_x, panel_y + 215, campo_w, 46,
        "Verificar", COLOR_EXITO, COLOR_EXITO);
    Boton btn_reenviar = boton_crear(campo_x, panel_y + 275, campo_w, 40,
        "Reenviar codigo", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_volver = boton_crear(campo_x, panel_y + 325, campo_w, 38,
        "Mas tarde", COLOR_PANEL, COLOR_BORDE);

    char mensaje[90] = "";
    SDL_Color color_msg = COLOR_TEXTO_SUAVE;

    SDL_Event e;
    Navegacion siguiente = NAV_VERIFICAR;

    while (siguiente == NAV_VERIFICAR && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            input_manejar_evento(&in_codigo, &e);

            int enter = (e.type == SDL_KEYDOWN &&
                         e.key.keysym.sym == SDLK_RETURN);

            if (boton_fue_clickeado(&btn_verificar, &e) || enter) {
                if (in_codigo.largo == 0) {
                    strcpy(mensaje, "Ingresa el codigo que recibiste");
                    color_msg = COLOR_PELIGRO;
                } else if (verificarCodigo(cliente, in_codigo.texto)) {
                    strcpy(mensaje, "Cuenta verificada! Ya podes iniciar sesion");
                    color_msg = COLOR_EXITO;
                    // mostrar un instante y volver al login
                    ventana_limpiar(v);
                    panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);
                    SDL_Rect area = { panel_x, panel_y + panel_h/2 - 20, panel_w, 40 };
                    texto_centrado(v, v->font_normal, mensaje, area, COLOR_EXITO);
                    ventana_presentar(v);
                    SDL_Delay(1400);
                    siguiente = NAV_LOGIN;
                } else {
                    strcpy(mensaje, "Codigo incorrecto");
                    color_msg = COLOR_PELIGRO;
                    input_limpiar(&in_codigo);
                }
            }

            if (boton_fue_clickeado(&btn_reenviar, &e)) {
                if (reenviarCodigo(cliente)) {
                    strcpy(mensaje, "Codigo reenviado a tu mail");
                    color_msg = COLOR_EXITO;
                } else {
                    strcpy(mensaje, "No se pudo reenviar (revisa la conexion)");
                    color_msg = COLOR_PELIGRO;
                }
                input_limpiar(&in_codigo);
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_LOGIN;
        }

        boton_actualizar_hover(&btn_verificar, mx, my);
        boton_actualizar_hover(&btn_reenviar, mx, my);
        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Verificar correo",
                      campo_x, panel_y + 35, COLOR_TEXTO);

        char sub[120];
        snprintf(sub, sizeof(sub), "Enviamos un codigo a:");
        texto_dibujar(v, v->font_chico, sub,
                      campo_x, panel_y + 85, COLOR_TEXTO_SUAVE);
        texto_dibujar(v, v->font_chico, cliente->mail,
                      campo_x, panel_y + 105, COLOR_TEXTO);

        input_dibujar(v, &in_codigo);
        boton_dibujar(v, &btn_verificar);
        boton_dibujar(v, &btn_reenviar);
        boton_dibujar(v, &btn_volver);

        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_chico, mensaje,
                          campo_x, panel_y + panel_h - 26, color_msg);
        }

        ventana_presentar(v);
    }

    return siguiente;
}