# ============================================================
# Makefile - Sistema Bancario (consola + grafico)
#   make / make run / make run-consola / make clean
# ============================================================

CC = gcc
INCLUDES = -Ilogica -Iui -Iui/pantallas -Iconfig -Icorreo
CFLAGS = -Wall $(INCLUDES) $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_ttf -lcurl

# Logica de negocio (incluye config, correo y sesion_smtp)
SRC_LOGICA = logica/cliente.c logica/login.c logica/validaciones.c \
             logica/cuenta.c logica/operaciones.c logica/historial.c \
             logica/menu.c logica/admin.c logica/sesion_smtp.c \
             config/config.c correo/correo.c

SRC_UI = ui/ventana.c ui/componentes.c ui/app.c \
         ui/pantallas/pantalla_login.c \
         ui/pantallas/pantalla_registro.c \
         ui/pantallas/pantalla_verificar.c \
         ui/pantallas/pantalla_menu.c \
         ui/pantallas/pantalla_perfil.c \
         ui/pantallas/pantalla_seleccion_cuenta.c \
         ui/pantallas/pantalla_cuenta.c \
         ui/pantallas/pantalla_transferir.c \
         ui/pantallas/pantalla_historial.c \
         ui/pantallas/pantalla_menu_admin.c \
         ui/pantallas/pantalla_admin_clientes.c \
         ui/pantallas/pantalla_admin_detalle.c \
         ui/pantallas/pantalla_admin_cuentas.c \
         ui/pantallas/pantalla_admin_movimientos.c \
         ui/pantallas/pantalla_admin_editar.c \
         ui/pantallas/pantalla_admin_crear.c

all: banco banco_consola

banco: main_grafico.c $(SRC_LOGICA) $(SRC_UI)
	$(CC) $(CFLAGS) main_grafico.c $(SRC_LOGICA) $(SRC_UI) -o banco $(LIBS)

# La consola tambien necesita config/correo (cliente.c los usa) y -lcurl
banco_consola: main_consola.c $(SRC_LOGICA)
	$(CC) -Wall -Ilogica -Iconfig -Icorreo main_consola.c $(SRC_LOGICA) -o banco_consola -lcurl

run: banco
	./banco

run-consola: banco_consola
	./banco_consola

clean:
	rm -f banco banco_consola

.PHONY: all run run-consola clean