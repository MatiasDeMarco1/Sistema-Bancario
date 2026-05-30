#include "pantalla_admin_cuentas.h"
#include "componentes.h"
#include "cuenta.h"
#include <stdio.h>
#include <string.h>

// De cuenta.c
int obtenerCuentasCliente(char *cuit, Cuenta cuentas[], int max);

#define MAX_CUENTAS 8

Navegacion pantalla_admin_cuentas(Ventana *v, Cliente *cliente) {
    Cuenta cuentas[MAX_CUENTAS];
    int n = obtenerCuentasCliente(cliente->cuit, cuentas, MAX_CUENTAS);

    int panel_w = 560;
    int panel_h = 460;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int card_w = panel_w - 80;
    int card_h = 100;
    int card_x = panel_x + 40;
    int card_y0 = panel_y + 110;
    int card_gap = 16;

    Boton btn_volver = boton_crear(card_x, panel_y + panel_h - 56,
        card_w, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    SDL_Event e;
    Navegacion siguiente = NAV_ADMIN_CUENTAS;

    while (siguiente == NAV_ADMIN_CUENTAS && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }
            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_ADMIN_DETALLE;
        }

        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        char titulo[140];
        snprintf(titulo, sizeof(titulo), "Cuentas de %s %s",
                 cliente->nombre, cliente->apellido);
        texto_dibujar(v, v->font_grande, titulo,
                      card_x, panel_y + 35, COLOR_TEXTO);

        if (n == 0) {
            texto_dibujar(v, v->font_normal, "Este cliente no tiene cuentas.",
                          card_x, card_y0, COLOR_TEXTO_SUAVE);
        } else {
            for (int i = 0; i < n; i++) {
                SDL_Rect card = { card_x, card_y0 + i * (card_h + card_gap),
                                  card_w, card_h };
                panel_dibujar(v, card, COLOR_FONDO, COLOR_BORDE);

                const char *moneda = (cuentas[i].moneda == PESOS)
                                     ? "Cuenta en Pesos" : "Cuenta en Dolares";
                texto_dibujar(v, v->font_normal, moneda,
                              card.x + 20, card.y + 14, COLOR_TEXTO);

                char linea[80];
                const char *simbolo = (cuentas[i].moneda == PESOS) ? "$" : "U$D ";
                snprintf(linea, sizeof(linea), "Saldo: %s%.2f",
                         simbolo, cuentas[i].saldo);
                texto_dibujar(v, v->font_normal, linea,
                              card.x + 20, card.y + 42, COLOR_EXITO);

                snprintf(linea, sizeof(linea), "CBU: %s", cuentas[i].cbu);
                texto_dibujar(v, v->font_chico, linea,
                              card.x + 20, card.y + 70, COLOR_TEXTO_SUAVE);
                snprintf(linea, sizeof(linea), "Alias: %s", cuentas[i].alias);
                texto_dibujar(v, v->font_chico, linea,
                              card.x + 300, card.y + 70, COLOR_TEXTO_SUAVE);
            }
        }

        boton_dibujar(v, &btn_volver);
        ventana_presentar(v);
    }

    return siguiente;
}