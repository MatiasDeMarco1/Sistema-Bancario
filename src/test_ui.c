#include "ui/ventana.h"
#include "ui/componentes.h"

int main(int argc, char *argv[]) {
    Ventana v;
    if (!ventana_init(&v)) return 1;

    // widgets de prueba
    Input campo = input_crear(350, 250, 200, 40, "Escribí algo", 0);
    Boton btn = boton_crear(350, 320, 200, 45, "PROBAR",
                            COLOR_PRIMARIO, COLOR_PRIMARIO_HOVER);

    SDL_Event e;
    while (v.corriendo) {
        int mx, my;
        SDL_GetMouseState(&mx, &my);

        // --- eventos ---
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) v.corriendo = 0;
            input_manejar_evento(&campo, &e);
            if (boton_fue_clickeado(&btn, &e))
                printf("Click! Texto: %s\n", campo.texto);
        }

        boton_actualizar_hover(&btn, mx, my);

        // --- dibujo ---
        ventana_limpiar(&v);
        input_dibujar(&v, &campo);
        boton_dibujar(&v, &btn);
        ventana_presentar(&v);
    }

    ventana_destruir(&v);
    return 0;
}