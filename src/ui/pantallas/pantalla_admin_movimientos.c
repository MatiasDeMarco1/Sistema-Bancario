#include "pantalla_admin_movimientos.h"
#include "componentes.h"
#include "operaciones.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

// De historial.c
int obtenerHistorialCliente(char *cuit, Movimiento movs[], int max);

#define MAX_MOVS 200

Navegacion pantalla_admin_movimientos(Ventana *v, Cliente *cliente) {
    static Movimiento movs[MAX_MOVS];
    int n = obtenerHistorialCliente(cliente->cuit, movs, MAX_MOVS);

    int panel_w = 620;
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
    Navegacion siguiente = NAV_ADMIN_MOVIMIENTOS;

    while (siguiente == NAV_ADMIN_MOVIMIENTOS && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

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
            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_ADMIN_DETALLE;
        }

        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        char titulo[140];
        snprintf(titulo, sizeof(titulo), "Movimientos de %s %s",
                 cliente->nombre, cliente->apellido);
        texto_dibujar(v, v->font_grande, titulo,
                      panel_x + 20, panel_y + 30, COLOR_TEXTO);

        if (n == 0) {
            texto_dibujar(v, v->font_normal, "Sin movimientos registrados.",
                          panel_x + 20, panel_y + 100, COLOR_TEXTO_SUAVE);
        } else {
            SDL_RenderSetClipRect(v->renderer, &area_lista);

            for (int i = 0; i < n; i++) {
                int item_y = area_lista.y + i * item_h - scroll;
                if (item_y + item_h < area_lista.y) continue;
                if (item_y > area_lista.y + area_lista.h) break;

                SDL_Rect item = { area_lista.x, item_y, area_lista.w, item_h - 8 };
                rect_relleno(v, item, COLOR_FONDO);

                Movimiento *m = &movs[i];
                int enviada = (strcmp(m->cuit_origen, cliente->cuit) == 0);
                const char *tipo_txt;
                char monto_txt[40];
                SDL_Color color_monto;
                const char *simbolo = (m->moneda == PESOS) ? "$" : "U$D ";

                if (m->tipo == INGRESO) {
                    tipo_txt = "Ingreso";
                    snprintf(monto_txt, sizeof(monto_txt), "+%s%.2f", simbolo, m->monto);
                    color_monto = COLOR_EXITO;
                } else if (m->tipo == EGRESO) {
                    tipo_txt = "Egreso";
                    snprintf(monto_txt, sizeof(monto_txt), "-%s%.2f", simbolo, m->monto);
                    color_monto = COLOR_PELIGRO;
                } else {
                    if (enviada) {
                        tipo_txt = "Transferencia enviada";
                        snprintf(monto_txt, sizeof(monto_txt), "-%s%.2f", simbolo, m->monto);
                        color_monto = COLOR_PELIGRO;
                    } else {
                        tipo_txt = "Transferencia recibida";
                        snprintf(monto_txt, sizeof(monto_txt), "+%s%.2f", simbolo, m->monto);
                        color_monto = COLOR_EXITO;
                    }
                }

                texto_dibujar(v, v->font_normal, tipo_txt,
                              item.x + 16, item.y + 12, COLOR_TEXTO);

                if (m->tipo == TRANSFERENCIA) {
                    char detalle[60];
                    snprintf(detalle, sizeof(detalle), "%s %s",
                             enviada ? "Para:" : "De:",
                             enviada ? m->cbu_destino : m->cbu_origen);
                    texto_dibujar(v, v->font_chico, detalle,
                                  item.x + 16, item.y + 40, COLOR_TEXTO_SUAVE);
                }

                int tw, th;
                TTF_SizeUTF8(v->font_normal, monto_txt, &tw, &th);
                texto_dibujar(v, v->font_normal, monto_txt,
                              item.x + item.w - tw - 16, item.y + 12, color_monto);

                char fecha_txt[32];
                struct tm *tm_info = localtime(&m->fecha);
                strftime(fecha_txt, sizeof(fecha_txt), "%d/%m/%Y %H:%M", tm_info);
                int fw, fh;
                TTF_SizeUTF8(v->font_chico, fecha_txt, &fw, &fh);
                texto_dibujar(v, v->font_chico, fecha_txt,
                              item.x + item.w - fw - 16, item.y + 42, COLOR_TEXTO_SUAVE);
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