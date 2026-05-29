#include "pantalla_seleccion_cuenta.h"
#include "componentes.h"
#include <stdio.h>
#include <string.h>

// De cuenta.c (recorda agregar la declaracion en cuenta.h)
int obtenerCuentasCliente(char *cuit, Cuenta cuentas[], int max);

#define MAX_CUENTAS 8

Navegacion pantalla_seleccion_cuenta(Ventana *v, Cliente *cliente,
                                     Cuenta *cuenta_elegida) {
    // Traer las cuentas del cliente
    Cuenta cuentas[MAX_CUENTAS];
    int n = obtenerCuentasCliente(cliente->cuit, cuentas, MAX_CUENTAS);

    // --- Layout ---
    int panel_w = 480;
    int panel_h = 440;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    // Tarjetas: una por cuenta
    int card_w = 400;
    int card_h = 90;
    int card_x = panel_x + (panel_w - card_w) / 2;
    int card_y0 = panel_y + 110;
    int card_gap = 16;

    SDL_Rect cards[MAX_CUENTAS];
    for (int i = 0; i < n; i++) {
        cards[i] = (SDL_Rect){ card_x, card_y0 + i * (card_h + card_gap),
                               card_w, card_h };
    }

    // Boton volver
    Boton btn_volver = boton_crear(card_x, panel_y + panel_h - 60,
        card_w, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    SDL_Event e;
    Navegacion siguiente = NAV_SELECCION_CUENTA;
    int hover_card = -1;   // que tarjeta tiene el mouse encima

    while (siguiente == NAV_SELECCION_CUENTA && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // Detectar hover de tarjetas
        hover_card = -1;
        for (int i = 0; i < n; i++) {
            if (punto_en_rect(mx, my, &cards[i])) hover_card = i;
        }

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            // Click en una tarjeta
            if (e.type == SDL_MOUSEBUTTONDOWN &&
                e.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n; i++) {
                    if (punto_en_rect(e.button.x, e.button.y, &cards[i])) {
                        *cuenta_elegida = cuentas[i];   // copiar la cuenta
                        siguiente = NAV_CUENTA;
                    }
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_MENU;
        }

        boton_actualizar_hover(&btn_volver, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Seleccionar cuenta",
                      panel_x + 40, panel_y + 40, COLOR_TEXTO);

        // Tarjetas de cuenta
        for (int i = 0; i < n; i++) {
            // Resaltar borde si el mouse esta encima
            SDL_Color borde = (hover_card == i) ? COLOR_PRIMARIO : COLOR_BORDE;
            panel_dibujar(v, cards[i], COLOR_FONDO, borde);

            // Moneda (titulo de la tarjeta)
            const char *moneda = (cuentas[i].moneda == PESOS)
                                 ? "Cuenta en Pesos" : "Cuenta en Dolares";
            texto_dibujar(v, v->font_normal, moneda,
                          cards[i].x + 20, cards[i].y + 16, COLOR_TEXTO);

            // Saldo
            char saldo[64];
            const char *simbolo = (cuentas[i].moneda == PESOS) ? "$" : "U$D ";
            snprintf(saldo, sizeof(saldo), "Saldo: %s%.2f",
                     simbolo, cuentas[i].saldo);
            texto_dibujar(v, v->font_chico, saldo,
                          cards[i].x + 20, cards[i].y + 48, COLOR_TEXTO_SUAVE);

            // Alias (a la derecha, en chico)
            texto_dibujar(v, v->font_chico, cuentas[i].alias,
                          cards[i].x + 20, cards[i].y + 66, COLOR_TEXTO_SUAVE);
        }

        // Si no tiene cuentas (no deberia pasar viniendo del menu, pero por las dudas)
        if (n == 0) {
            texto_dibujar(v, v->font_normal, "No tenes cuentas activas.",
                          panel_x + 40, card_y0, COLOR_TEXTO_SUAVE);
        }

        boton_dibujar(v, &btn_volver);
        ventana_presentar(v);
    }

    return siguiente;
}