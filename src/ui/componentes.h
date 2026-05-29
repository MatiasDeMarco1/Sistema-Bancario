#ifndef COMPONENTES_H
#define COMPONENTES_H

#include "ventana.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// ---------------------------------------------------------
// Estructuras de widgets
// ---------------------------------------------------------

// Botón clickeable
typedef struct {
    SDL_Rect  rect;
    char      texto[64];
    SDL_Color color_fondo;
    SDL_Color color_hover;
    int       hover;        // 1 si el mouse esta encima
} Boton;

// Campo de entrada de texto
typedef struct {
    SDL_Rect  rect;
    char      texto[128];   // contenido escrito
    char      placeholder[64];
    int       largo;        // cantidad de caracteres actuales
    int       activo;       // 1 si tiene el foco
    int       es_password;  // 1 oculta el texto con asteriscos
} Input;

// ---------------------------------------------------------
// Texto
// ---------------------------------------------------------

// Dibuja texto en (x, y), alineado a la izquierda
void texto_dibujar(Ventana *v, TTF_Font *font, const char *str,
                int x, int y, SDL_Color color);

// Dibuja texto centrado dentro de un rectangulo
void texto_centrado(Ventana *v, TTF_Font *font, const char *str,
                    SDL_Rect area, SDL_Color color);

// ---------------------------------------------------------
// Rectangulos
// ---------------------------------------------------------

void rect_relleno(Ventana *v, SDL_Rect rect, SDL_Color color);
void rect_borde(Ventana *v, SDL_Rect rect, SDL_Color color);
void panel_dibujar(Ventana *v, SDL_Rect rect, SDL_Color fondo, SDL_Color borde);

// ---------------------------------------------------------
// Boton
// ---------------------------------------------------------

Boton boton_crear(int x, int y, int ancho, int alto, const char *texto,
                SDL_Color fondo, SDL_Color hover);
void  boton_dibujar(Ventana *v, Boton *b);
void  boton_actualizar_hover(Boton *b, int mouse_x, int mouse_y);
int   boton_fue_clickeado(Boton *b, SDL_Event *e);

// ---------------------------------------------------------
// Input
// ---------------------------------------------------------

Input input_crear(int x, int y, int ancho, int alto,
                  const char *placeholder, int es_password);
void  input_dibujar(Ventana *v, Input *in);
void  input_manejar_evento(Input *in, SDL_Event *e);
void  input_limpiar(Input *in);

// ---------------------------------------------------------
// Utilidad
// ---------------------------------------------------------

int punto_en_rect(int x, int y, SDL_Rect *rect);

#endif