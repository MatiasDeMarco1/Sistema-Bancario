# ============================================================
# Makefile - Sistema Bancario con SDL2
#   make / make run / make clean
# ============================================================

CC = gcc
INCLUDES = -Ilogica -Iui -Iui/pantallas
CFLAGS = -Wall $(INCLUDES) $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

# Logica compartida por ambas versiones
SRC_LOGICA = logica/cliente.c logica/login.c logica/validaciones.c \
            logica/cuenta.c logica/operaciones.c logica/historial.c \
            logica/menu.c

# UI (solo la version grafica)
SRC_UI = ui/ventana.c ui/componentes.c ui/app.c \
        ui/pantallas/pantalla_login.c \
        ui/pantallas/pantalla_registro.c \
        ui/pantallas/pantalla_menu.c \
        ui/pantallas/pantalla_seleccion_cuenta.c \
        ui/pantallas/pantalla_cuenta.c \
        ui/pantallas/pantalla_transferir.c \
        ui/pantallas/pantalla_historial.c

# --- Objetivos ---
all: banco banco_consola

# Version grafica (logica + UI + SDL)
banco: main_grafico.c $(SRC_LOGICA) $(SRC_UI)
	$(CC) $(CFLAGS) main_grafico.c $(SRC_LOGICA) $(SRC_UI) -o banco $(LIBS)

# Version consola (solo logica, sin SDL)
banco_consola: main_consola.c $(SRC_LOGICA)
	$(CC) -Wall -Ilogica main_consola.c $(SRC_LOGICA) -o banco_consola

run: banco
	./banco

run-consola: banco_consola
	./banco_consola

clean:
	rm -f banco banco_consola

.PHONY: all run run-consola clean