# ============================================================
# Makefile - Sistema Bancario con SDL2
#   make / make run / make clean
# ============================================================
 
CC = gcc
 
INCLUDES = -Ilogica -Iui -Iui/pantallas
CFLAGS = -Wall $(INCLUDES) $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_ttf
 
# Logica de negocio
SRC_LOGICA = logica/cliente.c logica/login.c logica/validaciones.c \
             logica/cuenta.c logica/operaciones.c
 
# Capa de UI (SDL2)
SRC_UI = ui/ventana.c ui/componentes.c \
         ui/pantallas/pantalla_login.c \
         ui/pantallas/pantalla_menu.c \
         ui/pantallas/pantalla_seleccion_cuenta.c \
         ui/pantallas/pantalla_cuenta.c
 
SRC_MAIN = test_app.c
 
SRCS = $(SRC_MAIN) $(SRC_LOGICA) $(SRC_UI)
TARGET = test_app
 
all: $(TARGET)
 
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)
 
run: $(TARGET)
	./$(TARGET)
 
clean:
	rm -f $(TARGET)
 
.PHONY: all run clean
 