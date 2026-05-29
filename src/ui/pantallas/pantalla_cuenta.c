#include "pantalla_cuenta.h"
#include "componentes.h"
#include "operaciones.h"
#include "cuenta.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// De cuenta.c
void armarAlias(char *alias, const char *base, Moneda moneda);
void guardarCambiosCuenta(Cuenta *c);
int  aliasUnico(const char *alias);

// Sub-modos internos
typedef enum {
    MODO_VER,
    MODO_INGRESAR,
    MODO_RETIRAR,
    MODO_ALIAS
} ModoCuenta;

static const char *mensaje_resultado(ResultadoOp r) {
    switch (r) {
        case OP_OK:                   return "Operacion exitosa";
        case OP_MONTO_INVALIDO:       return "El monto debe ser mayor a 0";
        case OP_SALDO_INSUFICIENTE:   return "Saldo insuficiente";
        case OP_DESTINO_NO_ENCONTRADO:return "Cuenta destino no encontrada";
        case OP_MISMA_CUENTA:         return "No podes operar a tu propia cuenta";
        case OP_DISTINTA_MONEDA:      return "Distinta moneda";
        case OP_ERROR_ARCHIVO:        return "Error al abrir archivo";
        default:                      return "";
    }
}

Navegacion pantalla_cuenta(Ventana *v, Cuenta *cuenta) {
    int panel_w = 520;
    int panel_h = 460;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int btn_w = 210;
    int btn_h = 46;
    int gap   = 16;
    int col1  = panel_x + 40;
    int col2  = col1 + btn_w + gap;
    int fila1 = panel_y + 200;
    int fila2 = fila1 + btn_h + gap;

    Boton btn_ingresar = boton_crear(col1, fila1, btn_w, btn_h,
        "Ingresar dinero", COLOR_EXITO, COLOR_EXITO);
    Boton btn_retirar = boton_crear(col2, fila1, btn_w, btn_h,
        "Retirar dinero", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_transferir = boton_crear(col1, fila2, btn_w, btn_h,
        "Transferir", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_alias = boton_crear(col2, fila2, btn_w, btn_h,
        "Modificar alias", COLOR_PANEL, COLOR_BORDE);
    Boton btn_volver = boton_crear(col1, panel_y + panel_h - 60,
        btn_w * 2 + gap, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    // Formulario (monto o alias segun el modo)
    int form_y = fila2 + btn_h + 30;
    Input in_campo = input_crear(col1, form_y, 300, 42, "", 0);
    Boton btn_confirmar = boton_crear(col1 + 310, form_y, 130, 42,
        "Confirmar", COLOR_EXITO, COLOR_EXITO);

    ModoCuenta modo = MODO_VER;
    char mensaje[80] = "";
    SDL_Color color_msg = COLOR_TEXTO_SUAVE;

    SDL_Event e;
    Navegacion siguiente = NAV_CUENTA;

    while (siguiente == NAV_CUENTA && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            if (modo == MODO_VER) {
                if (boton_fue_clickeado(&btn_ingresar, &e)) {
                    modo = MODO_INGRESAR;
                    input_limpiar(&in_campo);
                    strcpy(in_campo.placeholder, "Monto");
                    in_campo.activo = 1;
                    mensaje[0] = '\0';
                }
                if (boton_fue_clickeado(&btn_retirar, &e)) {
                    modo = MODO_RETIRAR;
                    input_limpiar(&in_campo);
                    strcpy(in_campo.placeholder, "Monto");
                    in_campo.activo = 1;
                    mensaje[0] = '\0';
                }
                if (boton_fue_clickeado(&btn_transferir, &e))
                    siguiente = NAV_TRANSFERIR;
                if (boton_fue_clickeado(&btn_alias, &e)) {
                    modo = MODO_ALIAS;
                    input_limpiar(&in_campo);
                    strcpy(in_campo.placeholder, "Nuevo alias (sin sufijo)");
                    in_campo.activo = 1;
                    mensaje[0] = '\0';
                }
                if (boton_fue_clickeado(&btn_volver, &e))
                    siguiente = NAV_SELECCION_CUENTA;
            } else {
                input_manejar_evento(&in_campo, &e);

                int enter = (e.type == SDL_KEYDOWN &&
                             e.key.keysym.sym == SDLK_RETURN);
                int escape = (e.type == SDL_KEYDOWN &&
                              e.key.keysym.sym == SDLK_ESCAPE);

                if (escape) { modo = MODO_VER; mensaje[0] = '\0'; }

                if (boton_fue_clickeado(&btn_confirmar, &e) || enter) {
                    if (modo == MODO_ALIAS) {
                        if (in_campo.largo == 0) {
                            strcpy(mensaje, "El alias no puede estar vacio");
                            color_msg = COLOR_PELIGRO;
                        } else {
                            char alias_nuevo[50];
                            armarAlias(alias_nuevo, in_campo.texto, cuenta->moneda);
                            if (!aliasUnico(alias_nuevo)) {
                                strcpy(mensaje, "Ese alias ya esta en uso");
                                color_msg = COLOR_PELIGRO;
                            } else {
                                strcpy(cuenta->alias, alias_nuevo);
                                guardarCambiosCuenta(cuenta);
                                strcpy(mensaje, "Alias actualizado");
                                color_msg = COLOR_EXITO;
                                modo = MODO_VER;
                            }
                        }
                    } else {
                        double monto = atof(in_campo.texto);
                        ResultadoOp r;
                        if (modo == MODO_INGRESAR)
                            r = ingresarDinero_op(cuenta, monto);
                        else
                            r = retirarDinero_op(cuenta, monto);

                        strcpy(mensaje, mensaje_resultado(r));
                        color_msg = (r == OP_OK) ? COLOR_EXITO : COLOR_PELIGRO;
                        if (r == OP_OK) modo = MODO_VER;
                    }
                }
            }
        }

        if (modo == MODO_VER) {
            boton_actualizar_hover(&btn_ingresar, mx, my);
            boton_actualizar_hover(&btn_retirar, mx, my);
            boton_actualizar_hover(&btn_transferir, mx, my);
            boton_actualizar_hover(&btn_alias, mx, my);
            boton_actualizar_hover(&btn_volver, mx, my);
        } else {
            boton_actualizar_hover(&btn_confirmar, mx, my);
        }

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        char titulo[64];
        snprintf(titulo, sizeof(titulo), "Cuenta en %s",
                 cuenta->moneda == PESOS ? "Pesos" : "Dolares");
        texto_dibujar(v, v->font_grande, titulo,
                      panel_x + 40, panel_y + 30, COLOR_TEXTO);

        char linea[80];
        snprintf(linea, sizeof(linea), "CBU:   %s", cuenta->cbu);
        texto_dibujar(v, v->font_chico, linea,
                      panel_x + 40, panel_y + 80, COLOR_TEXTO_SUAVE);
        snprintf(linea, sizeof(linea), "Alias: %s", cuenta->alias);
        texto_dibujar(v, v->font_chico, linea,
                      panel_x + 40, panel_y + 102, COLOR_TEXTO_SUAVE);

        const char *simbolo = (cuenta->moneda == PESOS) ? "$" : "U$D ";
        snprintf(linea, sizeof(linea), "Saldo: %s%.2f", simbolo, cuenta->saldo);
        texto_dibujar(v, v->font_normal, linea,
                      panel_x + 40, panel_y + 135, COLOR_TEXTO);

        if (modo == MODO_VER) {
            boton_dibujar(v, &btn_ingresar);
            boton_dibujar(v, &btn_retirar);
            boton_dibujar(v, &btn_transferir);
            boton_dibujar(v, &btn_alias);
            boton_dibujar(v, &btn_volver);
        } else {
            const char *etiqueta;
            if (modo == MODO_INGRESAR)     etiqueta = "Monto a depositar:";
            else if (modo == MODO_RETIRAR) etiqueta = "Monto a extraer:";
            else                           etiqueta = "Nuevo alias:";
            texto_dibujar(v, v->font_normal, etiqueta, col1, fila1, COLOR_TEXTO);
            input_dibujar(v, &in_campo);
            boton_dibujar(v, &btn_confirmar);
            texto_dibujar(v, v->font_chico, "Enter para confirmar, Esc para cancelar",
                          col1, form_y + 56, COLOR_TEXTO_SUAVE);
        }

        if (mensaje[0] != '\0') {
            // En modo formulario el mensaje va debajo del texto de ayuda;
            // en modo VER va en la zona inferior (no molesta a los botones)
            int msg_y = (modo == MODO_VER)
                        ? panel_y + panel_h - 95
                        : form_y + 82;
            texto_dibujar(v, v->font_normal, mensaje,
                          panel_x + 40, msg_y, color_msg);
        }

        ventana_presentar(v);
    }

    return siguiente;
}