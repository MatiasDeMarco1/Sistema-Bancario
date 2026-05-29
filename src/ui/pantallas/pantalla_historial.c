#include "pantalla_historial.h"
#include "componentes.h"
#include "operaciones.h"
#include <stdio.h>
#include <string.h>

// De historial.c (recorda agregar la declaracion en historial.h)
int obtenerHistorialCliente(char *cuit, Movimiento movs[], int max);

#define MAX_MOVS 200

Navegacion pantalla_historial(Ventana *v, Cliente *cliente) {
    // Traer movimientos del cliente
    static Movimiento movs[MAX_MOVS];   // static: evita gran stack frame
    int n = obtenerHistorialCliente(cliente->cuit, movs, MAX_MOVS);

    // --- Layout ---
    int panel_w = 600;
    int panel_h = 500;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    // Area de lista (con recorte para el scroll)
    SDL_Rect area_lista = {
        panel_x + 20, panel_y + 80,
        panel_w - 40, panel_h - 150
    };

    int item_h = 70;          // alto de cada movimiento
    int scroll = 0;           // desplazamiento vertical en px
    int total_alto = n * item_h;
    int max_scroll = total_alto - area_lista.h;
    if (max_scroll < 0) max_scroll = 0;

    Boton btn_volver = boton_crear(panel_x + 20, panel_y + panel_h - 56,
        panel_w - 40, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    SDL_Event e;
    Navegacion siguiente = NAV_HISTORIAL;

    while (siguiente == NAV_HISTORIAL && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            // Scroll con la rueda del mouse
            if (e.type == SDL_MOUSEWHEEL) {
                scroll -= e.wheel.y * 30;   // 30 px por "tick"
                if (scroll < 0) scroll = 0;
                if (scroll > max_scroll) scroll = max_scroll;
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_MENU;
        }

        boton_actualizar_hover(&btn_volver, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Historial de movimientos",
                    panel_x + 20, panel_y + 30, COLOR_TEXTO);

        if (n == 0) {
            texto_dibujar(v, v->font_normal, "No hay movimientos registrados.",
                        panel_x + 20, panel_y + 100, COLOR_TEXTO_SUAVE);
        } else {
            // Recorte: solo dibujar dentro del area de lista
            SDL_RenderSetClipRect(v->renderer, &area_lista);

            for (int i = 0; i < n; i++) {
                int item_y = area_lista.y + i * item_h - scroll;

                // Saltar los que quedan fuera del area visible
                if (item_y + item_h < area_lista.y) continue;
                if (item_y > area_lista.y + area_lista.h) break;

                SDL_Rect item = { area_lista.x, item_y,
                                area_lista.w, item_h - 8 };
                rect_relleno(v, item, COLOR_FONDO);

                Movimiento *m = &movs[i];

                // Determinar tipo y signo respecto al cliente
                int enviada = (strcmp(m->cuit_origen, cliente->cuit) == 0);
                const char *tipo_txt;
                char monto_txt[40];
                SDL_Color color_monto;
                const char *simbolo = (m->moneda == PESOS) ? "$" : "U$D ";

                if (m->tipo == INGRESO) {
                    tipo_txt = "Ingreso";
                    snprintf(monto_txt, sizeof(monto_txt), "+%s%.2f",
                            simbolo, m->monto);
                    color_monto = COLOR_EXITO;
                } else if (m->tipo == EGRESO) {
                    tipo_txt = "Egreso";
                    snprintf(monto_txt, sizeof(monto_txt), "-%s%.2f",
                            simbolo, m->monto);
                    color_monto = COLOR_PELIGRO;
                } else {  // TRANSFERENCIA
                    if (enviada) {
                        tipo_txt = "Transferencia enviada";
                        snprintf(monto_txt, sizeof(monto_txt), "-%s%.2f",
                                simbolo, m->monto);
                        color_monto = COLOR_PELIGRO;
                    } else {
                        tipo_txt = "Transferencia recibida";
                        snprintf(monto_txt, sizeof(monto_txt), "+%s%.2f",
                                simbolo, m->monto);
                        color_monto = COLOR_EXITO;
                    }
                }

                // Tipo (izquierda arriba)
                texto_dibujar(v, v->font_normal, tipo_txt,
                            item.x + 16, item.y + 12, COLOR_TEXTO);

                // Detalle (CBU contraparte si es transferencia)
                if (m->tipo == TRANSFERENCIA) {
                    char detalle[60];
                    snprintf(detalle, sizeof(detalle), "%s %s",
                            enviada ? "Para:" : "De:",
                            enviada ? m->cbu_destino : m->cbu_origen);
                    texto_dibujar(v, v->font_chico, detalle,
                                item.x + 16, item.y + 40, COLOR_TEXTO_SUAVE);
                }

                // Monto (derecha)
                int tw, th;
                TTF_SizeUTF8(v->font_normal, monto_txt, &tw, &th);
                texto_dibujar(v, v->font_normal, monto_txt,
                            item.x + item.w - tw - 16, item.y + 12, color_monto);
            }

            // Quitar el recorte
            SDL_RenderSetClipRect(v->renderer, NULL);

            // Indicador de scroll (si hay mas de lo que entra)
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