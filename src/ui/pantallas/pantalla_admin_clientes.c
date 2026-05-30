#include "pantalla_admin_clientes.h"
#include "componentes.h"
#include <stdio.h>
#include <string.h>

// De cliente.c
int obtenerTodosLosClientes(Cliente arr[], int max);

#define MAX_CLIENTES 200

Navegacion pantalla_admin_clientes(Ventana *v, Cliente *seleccionado) {
    static Cliente clientes[MAX_CLIENTES];
    int n = obtenerTodosLosClientes(clientes, MAX_CLIENTES);

    int panel_w = 640;
    int panel_h = 520;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    SDL_Rect area_lista = {
        panel_x + 20, panel_y + 80,
        panel_w - 40, panel_h - 150
    };

    int item_h = 70;
    int scroll = 0;
    int total_alto = n * item_h;
    int max_scroll = total_alto - area_lista.h;
    if (max_scroll < 0) max_scroll = 0;

    Boton btn_volver = boton_crear(panel_x + 20, panel_y + panel_h - 56,
        panel_w - 40, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    SDL_Event e;
    Navegacion siguiente = NAV_ADMIN_CLIENTES;
    int hover_item = -1;

    while (siguiente == NAV_ADMIN_CLIENTES && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // Detectar hover de fila (solo dentro del area visible)
        hover_item = -1;
        for (int i = 0; i < n; i++) {
            int item_y = area_lista.y + i * item_h - scroll;
            if (item_y + item_h < area_lista.y) continue;
            if (item_y > area_lista.y + area_lista.h) break;
            SDL_Rect item = { area_lista.x, item_y, area_lista.w, item_h - 8 };
            if (punto_en_rect(mx, my, &item) &&
                my >= area_lista.y && my <= area_lista.y + area_lista.h)
                hover_item = i;
        }

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            if (e.type == SDL_MOUSEWHEEL) {
                scroll -= e.wheel.y * 30;
                if (scroll < 0) scroll = 0;
                if (scroll > max_scroll) scroll = max_scroll;
            }

            // Clic en una fila -> seleccionar cliente y ver detalle
            if (e.type == SDL_MOUSEBUTTONDOWN &&
                e.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n; i++) {
                    int item_y = area_lista.y + i * item_h - scroll;
                    if (item_y + item_h < area_lista.y) continue;
                    if (item_y > area_lista.y + area_lista.h) break;
                    SDL_Rect item = { area_lista.x, item_y, area_lista.w, item_h - 8 };
                    if (punto_en_rect(e.button.x, e.button.y, &item) &&
                        e.button.y >= area_lista.y &&
                        e.button.y <= area_lista.y + area_lista.h) {
                        *seleccionado = clientes[i];
                        siguiente = NAV_ADMIN_DETALLE;
                    }
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_MENU_ADMIN;
        }

        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Gestion de clientes",
                      panel_x + 20, panel_y + 30, COLOR_TEXTO);

        if (n == 0) {
            texto_dibujar(v, v->font_normal, "No hay clientes registrados.",
                          panel_x + 20, panel_y + 100, COLOR_TEXTO_SUAVE);
        } else {
            SDL_RenderSetClipRect(v->renderer, &area_lista);

            for (int i = 0; i < n; i++) {
                int item_y = area_lista.y + i * item_h - scroll;
                if (item_y + item_h < area_lista.y) continue;
                if (item_y > area_lista.y + area_lista.h) break;

                SDL_Rect item = { area_lista.x, item_y, area_lista.w, item_h - 8 };
                // Resaltar la fila bajo el mouse
                SDL_Color borde = (hover_item == i) ? COLOR_PRIMARIO : COLOR_BORDE;
                panel_dibujar(v, item, COLOR_FONDO, borde);

                Cliente *c = &clientes[i];

                char nombre[120];
                snprintf(nombre, sizeof(nombre), "%s %s", c->nombre, c->apellido);
                texto_dibujar(v, v->font_normal, nombre,
                              item.x + 16, item.y + 10, COLOR_TEXTO);

                char detalle[140];
                snprintf(detalle, sizeof(detalle), "CUIT: %s   |   %s",
                         c->cuit, c->activo ? "Activo" : "Dado de baja");
                texto_dibujar(v, v->font_chico, detalle,
                              item.x + 16, item.y + 38,
                              c->activo ? COLOR_TEXTO_SUAVE : COLOR_PELIGRO);

                // Indicador de que es clickeable
                texto_dibujar(v, v->font_chico, "Ver detalle >",
                              item.x + item.w - 110, item.y + 24, COLOR_PRIMARIO);
            }

            SDL_RenderSetClipRect(v->renderer, NULL);

            if (max_scroll > 0) {
                texto_dibujar(v, v->font_chico, "Scroll con la rueda del mouse",
                              panel_x + 20, panel_y + panel_h - 86,
                              COLOR_TEXTO_SUAVE);
            }
        }

        boton_dibujar(v, &btn_volver);
        ventana_presentar(v);
    }

    return siguiente;
}