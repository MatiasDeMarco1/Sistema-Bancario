#include "pantalla_admin_editar.h"
#include "componentes.h"
#include <string.h>
#include <stdio.h>

// De cliente.c (las nuevas + las que ya existian para "Mi perfil")
ResultadoEdicion editarNombre_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarApellido_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarMail_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarTelefono_op(Cliente *c, const char *nuevo);
ResultadoEdicion editarLocalidad_op(Cliente *c, const char *nueva);
ResultadoEdicion editarPais_op(Cliente *c, const char *nuevo);

// Campos editables por el admin
enum {
    CAMPO_NOMBRE, CAMPO_APELLIDO, CAMPO_MAIL,
    CAMPO_TELEFONO, CAMPO_LOCALIDAD, CAMPO_PAIS,
    CANT_CAMPOS
};

static const char *mensaje_edicion(ResultadoEdicion r) {
    switch (r) {
        case EDIT_OK:                return "Dato actualizado";
        case EDIT_MAIL_INVALIDO:     return "Mail invalido";
        case EDIT_MAIL_DUPLICADO:    return "Ese mail ya esta en uso";
        case EDIT_TELEFONO_INVALIDO: return "Telefono invalido";
        case EDIT_TELEFONO_DUPLICADO:return "Ese telefono ya esta en uso";
        case EDIT_VACIO:             return "El campo no puede estar vacio";
        default:                     return "";
    }
}

static ResultadoEdicion aplicar(Cliente *c, int campo, const char *valor) {
    switch (campo) {
        case CAMPO_NOMBRE:    return editarNombre_op(c, valor);
        case CAMPO_APELLIDO:  return editarApellido_op(c, valor);
        case CAMPO_MAIL:      return editarMail_op(c, valor);
        case CAMPO_TELEFONO:  return editarTelefono_op(c, valor);
        case CAMPO_LOCALIDAD: return editarLocalidad_op(c, valor);
        case CAMPO_PAIS:      return editarPais_op(c, valor);
        default:              return EDIT_VACIO;
    }
}

Navegacion pantalla_admin_editar(Ventana *v, Cliente *cliente) {
    int panel_w = 580;
    int panel_h = 560;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int x0 = panel_x + 40;
    int ancho = panel_w - 80;

    const char *etiquetas[CANT_CAMPOS] = {
        "Nombre", "Apellido", "Mail", "Telefono", "Localidad", "Pais"
    };

    int fila_y0 = panel_y + 130;
    int fila_h = 54;

    int btn_edit_w = 90;
    Boton btn_editar[CANT_CAMPOS];
    for (int i = 0; i < CANT_CAMPOS; i++) {
        btn_editar[i] = boton_crear(x0 + ancho - btn_edit_w,
                                    fila_y0 + i * fila_h, btn_edit_w, 36,
                                    "Editar", COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);
    }

    Boton btn_volver = boton_crear(x0, panel_y + panel_h - 56,
        ancho, 44, "Volver", COLOR_PANEL, COLOR_BORDE);

    int editando = -1;
    Input in_edit = input_crear(0, 0, 10, 10, "", 0);
    Boton btn_guardar = boton_crear(0, 0, 10, 10, "Guardar", COLOR_EXITO, COLOR_EXITO);
    Boton btn_cancelar_edit = boton_crear(0, 0, 10, 10, "Cancelar", COLOR_PANEL, COLOR_BORDE);

    char mensaje[80] = "";
    SDL_Color color_msg = COLOR_TEXTO_SUAVE;

    SDL_Event e;
    Navegacion siguiente = NAV_ADMIN_EDITAR;

    while (siguiente == NAV_ADMIN_EDITAR && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                siguiente = NAV_SALIR;
            }

            if (editando == -1) {
                for (int i = 0; i < CANT_CAMPOS; i++) {
                    if (boton_fue_clickeado(&btn_editar[i], &e)) {
                        editando = i;
                        int ey = fila_y0 + i * fila_h;
                        in_edit = input_crear(x0, ey, ancho - 200, 38,
                                              "Nuevo valor", 0);
                        in_edit.activo = 1;
                        btn_guardar = boton_crear(x0 + ancho - 190, ey, 90, 38,
                            "Guardar", COLOR_EXITO, COLOR_EXITO);
                        btn_cancelar_edit = boton_crear(x0 + ancho - 95, ey, 90, 38,
                            "Cancelar", COLOR_PANEL, COLOR_BORDE);
                        mensaje[0] = '\0';
                    }
                }
                if (boton_fue_clickeado(&btn_volver, &e))
                    siguiente = NAV_ADMIN_DETALLE;
            } else {
                input_manejar_evento(&in_edit, &e);

                int enter = (e.type == SDL_KEYDOWN &&
                             e.key.keysym.sym == SDLK_RETURN);
                int escape = (e.type == SDL_KEYDOWN &&
                              e.key.keysym.sym == SDLK_ESCAPE);

                if (escape || boton_fue_clickeado(&btn_cancelar_edit, &e)) {
                    editando = -1;
                    mensaje[0] = '\0';
                }

                if (boton_fue_clickeado(&btn_guardar, &e) || enter) {
                    ResultadoEdicion r = aplicar(cliente, editando, in_edit.texto);
                    strcpy(mensaje, mensaje_edicion(r));
                    color_msg = (r == EDIT_OK) ? COLOR_EXITO : COLOR_PELIGRO;
                    if (r == EDIT_OK) editando = -1;
                }
            }
        }

        if (editando == -1) {
            for (int i = 0; i < CANT_CAMPOS; i++)
                boton_actualizar_hover(&btn_editar[i], mx, my);
            boton_actualizar_hover(&btn_volver, mx, my);
        } else {
            boton_actualizar_hover(&btn_guardar, mx, my);
            boton_actualizar_hover(&btn_cancelar_edit, mx, my);
        }

        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Editar cliente",
                      x0, panel_y + 30, COLOR_TEXTO);
        char sub[80];
        snprintf(sub, sizeof(sub), "CUIT: %s  (no editable)", cliente->cuit);
        texto_dibujar(v, v->font_chico, sub, x0, panel_y + 80, COLOR_TEXTO_SUAVE);

        const char *valores[CANT_CAMPOS] = {
            cliente->nombre, cliente->apellido, cliente->mail,
            cliente->telefono, cliente->localidad, cliente->pais
        };

        for (int i = 0; i < CANT_CAMPOS; i++) {
            int ey = fila_y0 + i * fila_h;
            if (editando == i) {
                texto_dibujar(v, v->font_chico, etiquetas[i],
                              x0, ey - 16, COLOR_TEXTO_SUAVE);
                input_dibujar(v, &in_edit);
                boton_dibujar(v, &btn_guardar);
                boton_dibujar(v, &btn_cancelar_edit);
            } else {
                char linea[160];
                snprintf(linea, sizeof(linea), "%s: %s", etiquetas[i], valores[i]);
                texto_dibujar(v, v->font_normal, linea, x0, ey + 6, COLOR_TEXTO);
                if (editando == -1)
                    boton_dibujar(v, &btn_editar[i]);
                else {
                    rect_relleno(v, btn_editar[i].rect, COLOR_BORDE);
                    texto_centrado(v, v->font_chico, "Editar",
                                   btn_editar[i].rect, COLOR_TEXTO_SUAVE);
                }
            }
        }

        boton_dibujar(v, &btn_volver);

        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_chico, mensaje,
                          x0, panel_y + panel_h - 90, color_msg);
        }

        ventana_presentar(v);
    }

    return siguiente;
}