#include "componentes.h"
#include <string.h>
#include <stdio.h>

// =========================================================
// UTILIDAD
// =========================================================

int punto_en_rect(int x, int y, SDL_Rect *rect) {
    return (x >= rect->x && x <= rect->x + rect->w &&
            y >= rect->y && y <= rect->y + rect->h);
}

// =========================================================
// TEXTO
// =========================================================

void texto_dibujar(Ventana *v, TTF_Font *font, const char *str,
                int x, int y, SDL_Color color) {
    if (!font || !str || str[0] == '\0') return;

    SDL_Surface *sup = TTF_RenderUTF8_Blended(font, str, color);
    if (!sup) return;

    SDL_Texture *tex = SDL_CreateTextureFromSurface(v->renderer, sup);
    if (tex) {
        SDL_Rect dst = { x, y, sup->w, sup->h };
        SDL_RenderCopy(v->renderer, tex, NULL, &dst);
        SDL_DestroyTexture(tex);
    }
    SDL_FreeSurface(sup);
}

void texto_centrado(Ventana *v, TTF_Font *font, const char *str,
                    SDL_Rect area, SDL_Color color) {
    if (!font || !str || str[0] == '\0') return;

    int tw, th;
    TTF_SizeUTF8(font, str, &tw, &th);

    int x = area.x + (area.w - tw) / 2;
    int y = area.y + (area.h - th) / 2;
    texto_dibujar(v, font, str, x, y, color);
}

// =========================================================
// RECTANGULOS
// =========================================================

void rect_relleno(Ventana *v, SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(v->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(v->renderer, &rect);
}

void rect_borde(Ventana *v, SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(v->renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(v->renderer, &rect);
}

void panel_dibujar(Ventana *v, SDL_Rect rect, SDL_Color fondo, SDL_Color borde) {
    rect_relleno(v, rect, fondo);
    rect_borde(v, rect, borde);
}

// =========================================================
// BOTON
// =========================================================

Boton boton_crear(int x, int y, int ancho, int alto, const char *texto,
                SDL_Color fondo, SDL_Color hover) {
    Boton b;
    b.rect = (SDL_Rect){ x, y, ancho, alto };
    strncpy(b.texto, texto, sizeof(b.texto) - 1);
    b.texto[sizeof(b.texto) - 1] = '\0';
    b.color_fondo = fondo;
    b.color_hover = hover;
    b.hover = 0;
    return b;
}

void boton_dibujar(Ventana *v, Boton *b) {
    SDL_Color color = b->hover ? b->color_hover : b->color_fondo;
    rect_relleno(v, b->rect, color);
    texto_centrado(v, v->font_normal, b->texto, b->rect, COLOR_TEXTO);
}

void boton_actualizar_hover(Boton *b, int mouse_x, int mouse_y) {
    b->hover = punto_en_rect(mouse_x, mouse_y, &b->rect);
}

int boton_fue_clickeado(Boton *b, SDL_Event *e) {
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        return punto_en_rect(e->button.x, e->button.y, &b->rect);
    }
    return 0;
}

// =========================================================
// INPUT
// =========================================================

Input input_crear(int x, int y, int ancho, int alto,
                  const char *placeholder, int es_password) {
    Input in;
    in.rect = (SDL_Rect){ x, y, ancho, alto };
    in.texto[0] = '\0';
    strncpy(in.placeholder, placeholder, sizeof(in.placeholder) - 1);
    in.placeholder[sizeof(in.placeholder) - 1] = '\0';
    in.largo = 0;
    in.activo = 0;
    in.es_password = es_password;
    return in;
}

void input_dibujar(Ventana *v, Input *in) {
    // Fondo: cambia si esta activo
    SDL_Color fondo = in->activo ? COLOR_INPUT_ACTIVO : COLOR_INPUT;
    SDL_Color borde = in->activo ? COLOR_PRIMARIO : COLOR_BORDE;
    panel_dibujar(v, in->rect, fondo, borde);

    // Padding interno para el texto
    int tx = in->rect.x + 10;
    int ty = in->rect.y + (in->rect.h - 18) / 2;

    if (in->largo == 0) {
        // Mostrar placeholder en gris
        texto_dibujar(v, v->font_normal, in->placeholder, tx, ty, COLOR_TEXTO_SUAVE);
    } else if (in->es_password) {
        // Mostrar asteriscos
        char oculto[129];
        int i;
        for (i = 0; i < in->largo && i < 128; i++) oculto[i] = '*';
        oculto[i] = '\0';
        texto_dibujar(v, v->font_normal, oculto, tx, ty, COLOR_TEXTO);
    } else {
        texto_dibujar(v, v->font_normal, in->texto, tx, ty, COLOR_TEXTO);
    }
}

void input_manejar_evento(Input *in, SDL_Event *e) {
    // Click: activar o desactivar el foco
    if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {
        in->activo = punto_en_rect(e->button.x, e->button.y, &in->rect);
    }

    if (!in->activo) return;

    // Texto escrito
    if (e->type == SDL_TEXTINPUT) {
        int entrante = strlen(e->text.text);
        if (in->largo + entrante < (int)sizeof(in->texto) - 1) {
            strcat(in->texto, e->text.text);
            in->largo += entrante;
        }
    }

    // Backspace
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE) {
        if (in->largo > 0) {
            in->texto[--in->largo] = '\0';
        }
    }
}

void input_limpiar(Input *in) {
    in->texto[0] = '\0';
    in->largo = 0;
}