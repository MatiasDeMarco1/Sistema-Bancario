#include "pantalla_admin_detalle.h"
#include "componentes.h"
#include <stdio.h>
#include <string.h>

// De cliente.c
void eliminarCliente(char *cuit);
void reactivarCliente(char *cuit);

Navegacion pantalla_admin_detalle(Ventana *v, Cliente *cliente) {
    int panel_w = 560;
    int panel_h = 520;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int x0 = panel_x + 40;
    int ancho = panel_w - 80;

    int btn_h = 46;
    int gap = 14;
    int y0 = panel_y + 220;

    Boton btn_cuentas = boton_crear(x0, y0, ancho, btn_h,
        "Ver cuentas", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_movimientos = boton_crear(x0, y0 + (btn_h + gap), ancho, btn_h,
        "Ver movimientos", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_editar = boton_crear(x0, y0 + 2 * (btn_h + gap), ancho, btn_h,
        "Editar datos", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_estado = boton_crear(x0, y0 + 3 * (btn_h + gap), ancho, btn_h,
        "", COLOR_PELIGRO, COLOR_PELIGRO);
    Boton btn_volver = boton_crear(x0, y0 + 4 * (btn_h + gap), ancho, 44,
        "Volver a la lista", COLOR_PANEL, COLOR_BORDE);

    SDL_Event e;
    Navegacion siguiente = NAV_ADMIN_DETALLE;

    while (siguiente == NAV_ADMIN_DETALLE && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // El texto del boton de estado depende de si esta activo
        if (cliente->activo) {
            strcpy(btn_estado.texto, "Dar de baja");
            btn_estado.color_fondo = COLOR_PELIGRO;
            btn_estado.color_hover = COLOR_PELIGRO;
        } else {
            strcpy(btn_estado.texto, "Reactivar");
            btn_estado.color_fondo = COLOR_EXITO;
            btn_estado.color_hover = COLOR_EXITO;
        }

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            if (boton_fue_clickeado(&btn_cuentas, &e))
                siguiente = NAV_ADMIN_CUENTAS;
            if (boton_fue_clickeado(&btn_movimientos, &e))
                siguiente = NAV_ADMIN_MOVIMIENTOS;
            if (boton_fue_clickeado(&btn_editar, &e))
                siguiente = NAV_ADMIN_EDITAR;

            if (boton_fue_clickeado(&btn_estado, &e)) {
                if (cliente->activo) {
                    eliminarCliente(cliente->cuit);
                    cliente->activo = 0;
                } else {
                    reactivarCliente(cliente->cuit);
                    cliente->activo = 1;
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_ADMIN_CLIENTES;
        }

        boton_actualizar_hover(&btn_cuentas, mx, my);
        boton_actualizar_hover(&btn_movimientos, mx, my);
        boton_actualizar_hover(&btn_editar, mx, my);
        boton_actualizar_hover(&btn_estado, mx, my);
        boton_actualizar_hover(&btn_volver, mx, my);

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        // Datos del cliente
        char linea[140];
        snprintf(linea, sizeof(linea), "%s %s", cliente->nombre, cliente->apellido);
        texto_dibujar(v, v->font_grande, linea, x0, panel_y + 30, COLOR_TEXTO);

        snprintf(linea, sizeof(linea), "CUIT: %s", cliente->cuit);
        texto_dibujar(v, v->font_chico, linea, x0, panel_y + 80, COLOR_TEXTO_SUAVE);
        snprintf(linea, sizeof(linea), "Mail: %s", cliente->mail);
        texto_dibujar(v, v->font_chico, linea, x0, panel_y + 102, COLOR_TEXTO_SUAVE);
        snprintf(linea, sizeof(linea), "Telefono: %s", cliente->telefono);
        texto_dibujar(v, v->font_chico, linea, x0, panel_y + 124, COLOR_TEXTO_SUAVE);
        snprintf(linea, sizeof(linea), "Localidad: %s, %s",
                 cliente->localidad, cliente->pais);
        texto_dibujar(v, v->font_chico, linea, x0, panel_y + 146, COLOR_TEXTO_SUAVE);
        snprintf(linea, sizeof(linea), "Estado: %s",
                 cliente->activo ? "Activo" : "Dado de baja");
        texto_dibujar(v, v->font_normal, linea, x0, panel_y + 176,
                      cliente->activo ? COLOR_EXITO : COLOR_PELIGRO);

        boton_dibujar(v, &btn_cuentas);
        boton_dibujar(v, &btn_movimientos);
        boton_dibujar(v, &btn_editar);
        boton_dibujar(v, &btn_estado);
        boton_dibujar(v, &btn_volver);

        ventana_presentar(v);
    }

    return siguiente;
}