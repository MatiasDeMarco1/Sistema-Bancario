#include "pantalla_registro.h"
#include "componentes.h"
#include "cliente.h"
#include <string.h>
#include <stdio.h>

// De cliente.c
ResultadoRegistro crearCliente_op(Cliente *c);

// Indices de cada campo en el array de inputs
enum {
    F_NOMBRE, F_APELLIDO, F_CUIT, F_MAIL,
    F_TELEFONO, F_LOCALIDAD, F_PAIS, F_PASS,
    CANT_CAMPOS
};

static const char *mensaje_registro(ResultadoRegistro r) {
    switch (r) {
        case REG_OK:               return "Registro exitoso! Ya podes iniciar sesion";
        case REG_CUIT_INVALIDO:    return "CUIT invalido";
        case REG_CUIT_DUPLICADO:   return "Ya existe un cliente con ese CUIT";
        case REG_MAIL_INVALIDO:    return "Mail invalido";
        case REG_MAIL_DUPLICADO:   return "Ese mail ya esta registrado";
        case REG_TELEFONO_INVALIDO:return "Telefono invalido";
        case REG_TELEFONO_DUPLICADO:return "Ese telefono ya esta registrado";
        case REG_PASS_CORTA:       return "La contrasena debe tener al menos 8 caracteres";
        case REG_CAMPO_VACIO:      return "Complete todos los campos";
        default:                   return "";
    }
}

Navegacion pantalla_registro(Ventana *v, Cliente *registrado) {
    // --- Layout ---
    int panel_w = 560;
    int panel_h = 560;
    int panel_x = (VENTANA_ANCHO - panel_w) / 2;
    int panel_y = (VENTANA_ALTO  - panel_h) / 2;
    SDL_Rect panel = { panel_x, panel_y, panel_w, panel_h };

    int campo_w = panel_w - 80;
    int campo_x = panel_x + 40;
    int campo_h = 38;
    int gap = 50;            // separacion vertical entre campos
    int y0 = panel_y + 80;

    // Crear los 8 inputs (el ultimo es password)
    Input campos[CANT_CAMPOS];
    const char *placeholders[CANT_CAMPOS] = {
        "Nombre", "Apellido", "CUIT (XX-XXXXXXXX-X)", "Mail",
        "Telefono", "Localidad", "Pais", "Contrasena (min 8)"
    };
    for (int i = 0; i < CANT_CAMPOS; i++) {
        int es_pass = (i == F_PASS);
        campos[i] = input_crear(campo_x, y0 + i * gap, campo_w, campo_h,
                                placeholders[i], es_pass);
    }
    campos[F_NOMBRE].activo = 1;   // foco inicial en el primer campo

    // Botones
    int by = y0 + CANT_CAMPOS * gap + 10;
    int bw = (campo_w - 16) / 2;
    Boton btn_registrar = boton_crear(campo_x, by, bw, 44,
        "Registrarme", COLOR_EXITO, COLOR_EXITO);
    Boton btn_volver = boton_crear(campo_x + bw + 16, by, bw, 44,
        "Volver", COLOR_PANEL, COLOR_BORDE);

    char mensaje[80] = "";
    SDL_Color color_msg = COLOR_PELIGRO;

    SDL_Event e;
    Navegacion siguiente = NAV_LOGIN;   // por defecto, al salir vuelve al login
    int registrado_ok = 0;

    // Mientras no se haya registrado ni pedido salir, seguimos en el form.
    // Usamos una bandera local porque NAV_LOGIN es tambien el "estado actual".
    int en_pantalla = 1;

    while (en_pantalla && v->corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // ---------------- EVENTOS ----------------
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                v->corriendo = 0;
                en_pantalla = 0;
                siguiente = NAV_SALIR;
            }

            // Tab: pasar al siguiente campo
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_TAB) {
                int actual = -1;
                for (int i = 0; i < CANT_CAMPOS; i++)
                    if (campos[i].activo) { actual = i; break; }
                if (actual >= 0) campos[actual].activo = 0;
                int sig = (actual + 1) % CANT_CAMPOS;
                campos[sig].activo = 1;
            }

            // Pasar el evento a cada input
            for (int i = 0; i < CANT_CAMPOS; i++)
                input_manejar_evento(&campos[i], &e);

            // Registrar
            if (boton_fue_clickeado(&btn_registrar, &e)) {
                // Armar el cliente directamente en *registrado
                memset(registrado, 0, sizeof(Cliente));
                strncpy(registrado->nombre,    campos[F_NOMBRE].texto,    sizeof(registrado->nombre)-1);
                strncpy(registrado->apellido,  campos[F_APELLIDO].texto,  sizeof(registrado->apellido)-1);
                strncpy(registrado->cuit,      campos[F_CUIT].texto,      sizeof(registrado->cuit)-1);
                strncpy(registrado->mail,      campos[F_MAIL].texto,      sizeof(registrado->mail)-1);
                strncpy(registrado->telefono,  campos[F_TELEFONO].texto,  sizeof(registrado->telefono)-1);
                strncpy(registrado->localidad, campos[F_LOCALIDAD].texto, sizeof(registrado->localidad)-1);
                strncpy(registrado->pais,      campos[F_PAIS].texto,      sizeof(registrado->pais)-1);
                strncpy(registrado->contrasena,campos[F_PASS].texto,      sizeof(registrado->contrasena)-1);

                ResultadoRegistro r = crearCliente_op(registrado);
                strcpy(mensaje, mensaje_registro(r));

                if (r == REG_OK) {
                    color_msg = COLOR_EXITO;
                    registrado_ok = 1;
                } else {
                    color_msg = COLOR_PELIGRO;
                }
            }

            if (boton_fue_clickeado(&btn_volver, &e)) {
                en_pantalla = 0;
                siguiente = NAV_LOGIN;
            }
        }

        boton_actualizar_hover(&btn_registrar, mx, my);
        boton_actualizar_hover(&btn_volver, mx, my);

        // ---------------- DIBUJO ----------------
        ventana_limpiar(v);
        panel_dibujar(v, panel, COLOR_PANEL, COLOR_BORDE);

        texto_dibujar(v, v->font_grande, "Crear cuenta nueva",
                      campo_x, panel_y + 30, COLOR_TEXTO);

        for (int i = 0; i < CANT_CAMPOS; i++)
            input_dibujar(v, &campos[i]);

        boton_dibujar(v, &btn_registrar);
        boton_dibujar(v, &btn_volver);

        if (mensaje[0] != '\0') {
            texto_dibujar(v, v->font_chico, mensaje,
                          campo_x, by + 56, color_msg);
        }

        ventana_presentar(v);

        // Si se registro con exito, ir a la pantalla de verificacion
        if (registrado_ok) {
            SDL_Delay(800);
            en_pantalla = 0;
            siguiente = NAV_VERIFICAR;
        }
    }

    return siguiente;
}