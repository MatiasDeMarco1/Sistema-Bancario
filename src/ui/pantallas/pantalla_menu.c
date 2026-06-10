#include "pantalla_menu.h"
#include "componentes.h"
#include "cuenta.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// De cuenta.c / cuenta.h
int  clienteTieneCuenta(char *cuit, Moneda moneda);
void crearCuenta_op(Cuenta *c, char *cuit, Moneda moneda, const char *alias);
void armarAlias(char *alias, const char *base, Moneda moneda);
int  aliasUnico(const char *alias);

// De cliente.c
void eliminarCliente(char *cuit);

// Sub-modos del menu
typedef enum {
    MODO_MENU,          
    MODO_CREAR_PESOS,  
    MODO_CREAR_DOLARES, 
    MODO_CONFIRMAR_BAJA 
} ModoMenu;

Navegacion pantalla_menu(Ventana *v, Cliente *cliente) {
    int tienePesos   = clienteTieneCuenta(cliente->cuit, PESOS);
    int tieneDolares = clienteTieneCuenta(cliente->cuit, DOLARES);
    int tieneAlguna  = (tienePesos || tieneDolares);

    // --- Layout ---
    int panel_w = 460;
    int panel_h = 540;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int btn_w = 360;
    int btn_x = panel_x + (panel_w - btn_w) / 2;
    int btn_h = 42;
    int gap   = 11;
    int y0    = panel_y + 140;

    Boton btn_seleccionar = boton_crear(btn_x, y0, btn_w, btn_h,
        "Seleccionar cuenta", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_historial = boton_crear(btn_x, y0 + (btn_h + gap), btn_w, btn_h,
        "Ver historial", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_perfil = boton_crear(btn_x, y0 + 2 * (btn_h + gap), btn_w, btn_h,
        "Mi perfil", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    Boton btn_crear_pesos = boton_crear(btn_x, y0 + 3 * (btn_h + gap), btn_w, btn_h,
        "Crear cuenta en Pesos", COLOR_EXITO, COLOR_EXITO);
    Boton btn_crear_dolares = boton_crear(btn_x, y0 + 4 * (btn_h + gap), btn_w, btn_h,
        "Crear cuenta en Dolares", COLOR_EXITO, COLOR_EXITO);
    Boton btn_baja = boton_crear(btn_x, y0 + 5 * (btn_h + gap), btn_w, btn_h,
        "Darme de baja", COLOR_PELIGRO, COLOR_PELIGRO);
    Boton btn_salir = boton_crear(btn_x, y0 + 6 * (btn_h + gap), btn_w, btn_h,
        "Cerrar sesion", COLOR_PANEL, COLOR_BORDE);

    // Formulario de alias (para crear cuenta)
    Input in_alias = input_crear(btn_x, y0 + 40, btn_w, 42, "Alias (sin sufijo)", 0);
    Boton btn_confirmar = boton_crear(btn_x, y0 + 120, btn_w, 44,
        "Crear", COLOR_EXITO, COLOR_EXITO);
    Boton btn_cancelar = boton_crear(btn_x, y0 + 175, btn_w, 40,
        "Cancelar", COLOR_PANEL, COLOR_BORDE);

    // Botones del dialogo de confirmacion de baja
    Boton btn_baja_si = boton_crear(btn_x, y0 + 110, btn_w, 46,
        "Si, dar de baja", COLOR_PELIGRO, COLOR_PELIGRO);
    Boton btn_baja_no = boton_crear(btn_x, y0 + 166, btn_w, 44,
        "Cancelar", COLOR_PANEL, COLOR_BORDE);

    ModoMenu modo = MODO_MENU;
    char saludo[120];
    snprintf(saludo, sizeof(saludo), "Hola, %s %s",
             cliente->nombre, cliente->apellido);
    char mensaje[80] = "";

    SDL_Event e;
    Navegacion siguiente = NAV_MENU;

    while (siguiente == NAV_MENU && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            if (modo == MODO_MENU) {
                if (tieneAlguna && boton_fue_clickeado(&btn_seleccionar, &e))
                    siguiente = NAV_SELECCION_CUENTA;
                if (tieneAlguna && boton_fue_clickeado(&btn_historial, &e))
                    siguiente = NAV_HISTORIAL;

                if (boton_fue_clickeado(&btn_perfil, &e))
                    siguiente = NAV_PERFIL;

                if (!tienePesos && boton_fue_clickeado(&btn_crear_pesos, &e)) {
                    modo = MODO_CREAR_PESOS;
                    input_limpiar(&in_alias);
                    in_alias.activo = 1;
                    mensaje[0] = '\0';
                }
                if (!tieneDolares && boton_fue_clickeado(&btn_crear_dolares, &e)) {
                    modo = MODO_CREAR_DOLARES;
                    input_limpiar(&in_alias);
                    in_alias.activo = 1;
                    mensaje[0] = '\0';
                }

                if (boton_fue_clickeado(&btn_baja, &e)) {
                    modo = MODO_CONFIRMAR_BAJA;   
                    mensaje[0] = '\0';
                }
                if (boton_fue_clickeado(&btn_salir, &e))
                    siguiente = NAV_LOGIN;
            } else if (modo == MODO_CONFIRMAR_BAJA) {
                // Dialogo de confirmacion de baja
                int escape = (e.type == SDL_KEYDOWN &&
                              e.key.keysym.sym == SDLK_ESCAPE);
                if (escape || boton_fue_clickeado(&btn_baja_no, &e)) {
                    modo = MODO_MENU;
                }
                if (boton_fue_clickeado(&btn_baja_si, &e)) {
                    eliminarCliente(cliente->cuit);   
                    siguiente = NAV_LOGIN;            
                }
            } else {
                // Modo formulario: capturar alias
                input_manejar_evento(&in_alias, &e);

                int escape = (e.type == SDL_KEYDOWN &&
                              e.key.keysym.sym == SDLK_ESCAPE);
                if (escape || boton_fue_clickeado(&btn_cancelar, &e)) {
                    modo = MODO_MENU;
                    mensaje[0] = '\0';
                }

                int enter = (e.type == SDL_KEYDOWN &&
                             e.key.keysym.sym == SDLK_RETURN);
                if (boton_fue_clickeado(&btn_confirmar, &e) || enter) {
                    if (in_alias.largo == 0) {
                        strcpy(mensaje, "El alias no puede estar vacio");
                    } else {
                        Moneda m = (modo == MODO_CREAR_PESOS) ? PESOS : DOLARES;
                        char alias_final[50];
                        armarAlias(alias_final, in_alias.texto, m);

                        if (!aliasUnico(alias_final)) {
                            strcpy(mensaje, "Ese alias ya esta en uso");
                        } else {
                            Cuenta nueva;
                            crearCuenta_op(&nueva, cliente->cuit, m, alias_final);

                            // Actualizar estado
                            if (m == PESOS) tienePesos = 1;
                            else            tieneDolares = 1;
                            tieneAlguna = 1;

                            modo = MODO_MENU;
                            mensaje[0] = '\0';
                        }
                    }
                }
            }
        }

        // Hover
        if (modo == MODO_MENU) {
            if (tieneAlguna) {
                boton_actualizar_hover(&btn_seleccionar, mx, my);
                boton_actualizar_hover(&btn_historial, mx, my);
            }
            boton_actualizar_hover(&btn_perfil, mx, my);
            if (!tienePesos)   boton_actualizar_hover(&btn_crear_pesos, mx, my);
            if (!tieneDolares) boton_actualizar_hover(&btn_crear_dolares, mx, my);
            boton_actualizar_hover(&btn_baja, mx, my);
            boton_actualizar_hover(&btn_salir, mx, my);
        } else if (modo == MODO_CONFIRMAR_BAJA) {
            boton_actualizar_hover(&btn_baja_si, mx, my);
            boton_actualizar_hover(&btn_baja_no, mx, my);
        } else {
            boton_actualizar_hover(&btn_confirmar, mx, my);
            boton_actualizar_hover(&btn_cancelar, mx, my);
        }

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, saludo,
                      panel_x + 40, panel_y + 40, COLOR_TEXTO);

        if (modo == MODO_MENU) {
            char estado[120];
            if (tieneAlguna)
                snprintf(estado, sizeof(estado), "Cuentas: %s%s%s",
                         tienePesos ? "Pesos" : "",
                         (tienePesos && tieneDolares) ? " y " : "",
                         tieneDolares ? "Dolares" : "");
            else
                strcpy(estado, "Todavia no tenes cuentas");
            texto_dibujar(v, v->font_chico, estado,
                          panel_x + 40, panel_y + 90, COLOR_TEXTO_SUAVE);

            // Botones de cuentas (gris si no tiene)
            if (tieneAlguna) {
                boton_dibujar(v, &btn_seleccionar);
                boton_dibujar(v, &btn_historial);
            } else {
                rect_relleno(v, btn_seleccionar.rect, COLOR_BORDE);
                texto_centrado(v, v->font_normal, btn_seleccionar.texto,
                               btn_seleccionar.rect, COLOR_TEXTO_SUAVE);
                rect_relleno(v, btn_historial.rect, COLOR_BORDE);
                texto_centrado(v, v->font_normal, btn_historial.texto,
                               btn_historial.rect, COLOR_TEXTO_SUAVE);
            }

            boton_dibujar(v, &btn_perfil);

            // Botones de crear solo si falta esa moneda
            if (!tienePesos)   boton_dibujar(v, &btn_crear_pesos);
            if (!tieneDolares) boton_dibujar(v, &btn_crear_dolares);

            boton_dibujar(v, &btn_baja);
            boton_dibujar(v, &btn_salir);
        } else if (modo == MODO_CONFIRMAR_BAJA) {
            texto_dibujar(v, v->font_normal,
                          "Estas seguro que queres darte de baja?",
                          btn_x, y0, COLOR_TEXTO);
            texto_dibujar(v, v->font_chico,
                          "Esta accion desactiva tu cuenta de cliente.",
                          btn_x, y0 + 40, COLOR_TEXTO_SUAVE);
            boton_dibujar(v, &btn_baja_si);
            boton_dibujar(v, &btn_baja_no);
        } else {
            const char *etiqueta = (modo == MODO_CREAR_PESOS)
                ? "Nueva cuenta en Pesos - elegi un alias:"
                : "Nueva cuenta en Dolares - elegi un alias:";
            texto_dibujar(v, v->font_normal, etiqueta,
                          btn_x, y0, COLOR_TEXTO);

            const char *sufijo = (modo == MODO_CREAR_PESOS)
                ? "Se agregara .ars al final"
                : "Se agregara .usd al final";
            input_dibujar(v, &in_alias);
            texto_dibujar(v, v->font_chico, sufijo,
                          btn_x, y0 + 88, COLOR_TEXTO_SUAVE);

            boton_dibujar(v, &btn_confirmar);
            boton_dibujar(v, &btn_cancelar);
        }

        // Mensaje de error
        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_chico, mensaje,
                          panel_x + 40, panel_y + panel_h - 28, COLOR_PELIGRO);
        }

        ventana_presentar(v);
    }

    return siguiente;
}