#include "pantalla_login.h"
#include "componentes.h"
#include <string.h>

// Declaracion de la funcion de validacion que vive en login.c
// (recorda agregar tambien esta linea en login.h)
int login_validar(const char *identificador, const char *contrasena, Cliente *c);

int pantalla_login(Ventana *v, Cliente *c) {
    // --- Layout: panel centrado ---
    int panel_w = 380;
    int panel_h = 360;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;

    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    // --- Inputs ---
    int campo_w = 300;
    int campo_x = panel_x + (panel_w - campo_w) / 2;

    Input in_id = input_crear(campo_x, panel_y + 110, campo_w, 42,
                              "CUIT, mail o telefono", 0);
    Input in_pass = input_crear(campo_x, panel_y + 175, campo_w, 42,
                                "Contrasena", 1);

    // --- Boton ---
    Boton btn = boton_crear(campo_x, panel_y + 245, campo_w, 46, "INGRESAR",
                            COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);

    // Mensaje de error (vacio al inicio)
    char mensaje[80] = "";

    SDL_Event e;
    int resultado = 0;   // 0 = salio sin loguear, 1 = login OK
    int terminar = 0;

    while (!terminar && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                terminar = 1;
            }

            input_manejar_evento(&in_id, &e);
            input_manejar_evento(&in_pass, &e);

            // Enter tambien intenta loguear
            int enter = (e.type == SDL_KEYDOWN &&
                         e.key.keysym.sym == SDLK_RETURN);

            if (boton_fue_clickeado(&btn, &e) || enter) {
                if (in_id.largo == 0 || in_pass.largo == 0) {
                    strcpy(mensaje, "Complete todos los campos");
                } else if (login_validar(in_id.texto, in_pass.texto, c)) {
                    resultado = 1;
                    terminar = 1;
                } else {
                    strcpy(mensaje, "Credenciales incorrectas");
                    input_limpiar(&in_pass);   // limpiar solo el pass
                }
            }
        }

        boton_actualizar_hover(&btn, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);

        // Panel
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        // Titulo centrado arriba del panel
        SDL_Rect area_titulo = { panel_x, panel_y + 30, panel_w, 40 };
        texto_centrado(v, v->font_grande, "Sistema Bancario",
                       area_titulo, COLOR_TEXTO);

        SDL_Rect area_sub = { panel_x, panel_y + 70, panel_w, 24 };
        texto_centrado(v, v->font_chico, "Inicie sesion para continuar",
                       area_sub, COLOR_TEXTO_SUAVE);

        // Inputs y boton
        input_dibujar(v, &in_id);
        input_dibujar(v, &in_pass);
        boton_dibujar(v, &btn);

        // Mensaje de error (si hay)
        if (mensaje[0] != '\0') {
            SDL_Rect area_msg = { panel_x, panel_y + 300, panel_w, 24 };
            texto_centrado(v, v->font_chico, mensaje, area_msg, COLOR_PELIGRO);
        }

        ventana_presentar(v);
    }

    return resultado;
}