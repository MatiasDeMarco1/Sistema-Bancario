#include "pantalla_transferir.h"
#include "componentes.h"
#include "operaciones.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Traduce un ResultadoOp a mensaje (mismo criterio que pantalla_cuenta)
static const char *mensaje_resultado(ResultadoOp r) {
    switch (r) {
        case OP_OK:                    return "Transferencia exitosa";
        case OP_MONTO_INVALIDO:        return "El monto debe ser mayor a 0";
        case OP_SALDO_INSUFICIENTE:    return "Saldo insuficiente";
        case OP_DESTINO_NO_ENCONTRADO: return "Cuenta destino no encontrada";
        case OP_MISMA_CUENTA:          return "No podes transferir a tu propia cuenta";
        case OP_DISTINTA_MONEDA:       return "Distinta moneda";
        case OP_ERROR_ARCHIVO:         return "Error al abrir archivo";
        default:                       return "";
    }
}

Navegacion pantalla_transferir(Ventana *v, Cuenta *origen) {
    // --- Layout ---
    int panel_w = 520;
    int panel_h = 460;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int x0 = panel_x + 40;
    int campo_w = panel_w - 80;

    // Selector de metodo: dos botones (CBU / Alias)
    int sel_w = (campo_w - 16) / 2;
    Boton btn_cbu = boton_crear(x0, panel_y + 110, sel_w, 42,
        "Por CBU", COLOR_PANEL, COLOR_BORDE);
    Boton btn_alias = boton_crear(x0 + sel_w + 16, panel_y + 110, sel_w, 42,
        "Por Alias", COLOR_PANEL, COLOR_BORDE);

    // Inputs
    Input in_destino = input_crear(x0, panel_y + 200, campo_w, 42,
        "CBU o alias destino", 0);
    Input in_monto = input_crear(x0, panel_y + 280, campo_w, 42,
        "Monto", 0);

    // Acciones
    Boton btn_confirmar = boton_crear(x0, panel_y + 350, sel_w, 46,
        "Transferir", COLOR_EXITO, COLOR_EXITO);
    Boton btn_volver = boton_crear(x0 + sel_w + 16, panel_y + 350, sel_w, 46,
        "Volver", COLOR_PANEL, COLOR_BORDE);

    int metodo = 1;            // 1 = CBU, 2 = alias (arranca en CBU)
    char mensaje[80] = "";
    SDL_Color color_msg = COLOR_TEXTO_SUAVE;

    SDL_Event e;
    Navegacion siguiente = NAV_TRANSFERIR;

    while (siguiente == NAV_TRANSFERIR && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            // Selector de metodo
            if (boton_fue_clickeado(&btn_cbu, &e)) {
                metodo = 1;
                strcpy(in_destino.placeholder, "CBU destino (22 digitos)");
            }
            if (boton_fue_clickeado(&btn_alias, &e)) {
                metodo = 2;
                strcpy(in_destino.placeholder, "Alias destino");
            }

            // Inputs
            input_manejar_evento(&in_destino, &e);
            input_manejar_evento(&in_monto, &e);

            // Confirmar
            if (boton_fue_clickeado(&btn_confirmar, &e)) {
                if (in_destino.largo == 0 || in_monto.largo == 0) {
                    strcpy(mensaje, "Complete destino y monto");
                    color_msg = COLOR_PELIGRO;
                } else {
                    double monto = atof(in_monto.texto);
                    ResultadoOp r = transferir_op(origen, metodo,
                                                in_destino.texto, monto);
                    strcpy(mensaje, mensaje_resultado(r));
                    color_msg = (r == OP_OK) ? COLOR_EXITO : COLOR_PELIGRO;

                    if (r == OP_OK) {
                        // Limpiar campos para otra transferencia
                        input_limpiar(&in_destino);
                        input_limpiar(&in_monto);
                    }
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e))
                siguiente = NAV_CUENTA;
        }

        boton_actualizar_hover(&btn_confirmar, mx, my);
        boton_actualizar_hover(&btn_volver, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Transferir",
                    x0, panel_y + 30, COLOR_TEXTO);

        // Saldo disponible
        char saldo[80];
        const char *simbolo = (origen->moneda == PESOS) ? "$" : "U$D ";
        snprintf(saldo, sizeof(saldo), "Disponible: %s%.2f",
                simbolo, origen->saldo);
        texto_dibujar(v, v->font_chico, saldo,
                    x0, panel_y + 75, COLOR_TEXTO_SUAVE);

        // Selector de metodo: resaltar el activo
        btn_cbu.color_fondo   = (metodo == 1) ? COLOR_PRIMARIO : COLOR_PANEL;
        btn_alias.color_fondo = (metodo == 2) ? COLOR_PRIMARIO : COLOR_PANEL;
        boton_dibujar(v, &btn_cbu);
        boton_dibujar(v, &btn_alias);

        // Inputs con etiquetas
        texto_dibujar(v, v->font_chico, "Destino:",
                    x0, panel_y + 178, COLOR_TEXTO_SUAVE);
        input_dibujar(v, &in_destino);

        texto_dibujar(v, v->font_chico, "Monto:",
                    x0, panel_y + 258, COLOR_TEXTO_SUAVE);
        input_dibujar(v, &in_monto);

        boton_dibujar(v, &btn_confirmar);
        boton_dibujar(v, &btn_volver);

        // Mensaje
        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_normal, mensaje,
                        x0, panel_y + panel_h - 28, color_msg);
        }

        ventana_presentar(v);
    }

    return siguiente;
}