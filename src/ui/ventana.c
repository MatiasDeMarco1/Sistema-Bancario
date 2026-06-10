
#include "ventana.h"
#include <stdio.h>

int ventana_init(Ventana *v) {
    // Inicializar SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error SDL_Init: %s\n", SDL_GetError());
        return 0;
    }

    // Inicializar SDL_ttf (para texto)
    if (TTF_Init() != 0) {
        printf("Error TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    // Crear ventana (RESIZABLE para permitir maximizar)
    v->window = SDL_CreateWindow(
        VENTANA_TITULO,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        VENTANA_ANCHO,
        VENTANA_ALTO,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!v->window) {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Crear renderer con aceleración de hardware
    v->renderer = SDL_CreateRenderer(
        v->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!v->renderer) {
        printf("Error al crear renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(v->window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    // Lienzo logico fijo: SDL escala todo el contenido automaticamente al
    // tamaño real de la ventana, manteniendo la proporcion. Si la ventana
    // maximizada no respeta la proporcion 900x600, aparecen franjas (letterbox).
    SDL_RenderSetLogicalSize(v->renderer, VENTANA_ANCHO, VENTANA_ALTO);

    // Color de las franjas del letterbox (negro)
    SDL_SetRenderDrawColor(v->renderer, 0, 0, 0, 255);

    SDL_StartTextInput();

    // Cargar fuentes
    // IMPORTANTE: ajustar la ruta según donde tengan el .ttf
    v->font_grande = TTF_OpenFont("assets/fonts/font.ttf", 28);
    v->font_normal = TTF_OpenFont("assets/fonts/font.ttf", 18);
    v->font_chico  = TTF_OpenFont("assets/fonts/font.ttf", 14);

    if (!v->font_grande || !v->font_normal || !v->font_chico) {
        printf("Error al cargar fuente: %s\n", TTF_GetError());
        printf("Asegurate de tener assets/fonts/font.ttf\n");
        // No es fatal, pero el texto no va a renderizar
    }

    v->corriendo = 1;
    return 1;
}

void ventana_destruir(Ventana *v) {
    if (v->font_chico)  TTF_CloseFont(v->font_chico);
    if (v->font_normal) TTF_CloseFont(v->font_normal);
    if (v->font_grande) TTF_CloseFont(v->font_grande);
    if (v->renderer)    SDL_DestroyRenderer(v->renderer);
    if (v->window)      SDL_DestroyWindow(v->window);
    TTF_Quit();
    SDL_Quit();
}

void ventana_limpiar(Ventana *v) {
    SDL_Color c = COLOR_FONDO;
    SDL_SetRenderDrawColor(v->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(v->renderer);
}

void ventana_presentar(Ventana *v) {
    SDL_RenderPresent(v->renderer);
}

void ventana_set_color(Ventana *v, SDL_Color c) {
    SDL_SetRenderDrawColor(v->renderer, c.r, c.g, c.b, c.a);
}