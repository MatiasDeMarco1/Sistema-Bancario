#ifndef VENTANA_H
#define VENTANA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Dimensiones de la ventana
#define VENTANA_ANCHO  900
#define VENTANA_ALTO   600
#define VENTANA_TITULO "Sistema Bancario"

// Paleta de colores del sistema
#define COLOR_FONDO         (SDL_Color){15,  23,  42,  255}   // azul muy oscuro
#define COLOR_PANEL         (SDL_Color){30,  41,  59,  255}   // azul oscuro
#define COLOR_BORDE         (SDL_Color){51,  65,  85,  255}   // gris azulado
#define COLOR_PRIMARIO      (SDL_Color){59,  130, 246, 255}   // azul claro
#define COLOR_PRIMARIO_HOVER (SDL_Color){37, 99,  235, 255}   // azul más oscuro
#define COLOR_EXITO         (SDL_Color){34,  197, 94,  255}   // verde
#define COLOR_PELIGRO       (SDL_Color){239, 68,  68,  255}   // rojo
#define COLOR_TEXTO         (SDL_Color){241, 245, 249, 255}   // blanco suave
#define COLOR_TEXTO_SUAVE   (SDL_Color){148, 163, 184, 255}   // gris claro
#define COLOR_INPUT         (SDL_Color){15,  23,  42,  255}   // igual que fondo
#define COLOR_INPUT_ACTIVO  (SDL_Color){30,  41,  59,  255}   // panel

// Estructura principal de la ventana
typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    TTF_Font     *font_grande;    // títulos
    TTF_Font     *font_normal;    // texto general
    TTF_Font     *font_chico;     // texto secundario
    int           corriendo;
} Ventana;

// Funciones
int     ventana_init(Ventana *v);
void    ventana_destruir(Ventana *v);
void    ventana_limpiar(Ventana *v);
void    ventana_presentar(Ventana *v);
void    ventana_set_color(Ventana *v, SDL_Color c);

#endif