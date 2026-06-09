
# Sistema Bancario en C — Interfaz dual (consola y gráfica con SDL2)

Trabajo práctico final integrador de la materia Lenguajes Estructurados (UCEL).
Sistema de gestión bancaria escrito en C que combina manejo de archivos
binarios con operaciones de ABM, autenticación, roles y verificación de correo.

Su rasgo distintivo es que ofrece **dos interfaces** —una de consola y una
gráfica con SDL2— construidas sobre **la misma lógica de negocio**.

Integrantes: 
Leonardi Chiara,
Rossi Damian,
De Marco Matías 
---


## Características

- Registro y login de clientes con contraseñas hasheadas.
- Operaciones bancarias: ingresar, retirar y transferir (cuentas en pesos y dólares).
- Historial de movimientos con fecha y hora.
- Rol administrador: gestión de clientes, ver cuentas y movimientos, edición y baja/reactivación, alta de administradores.
- Verificación de correo electrónico real por SMTP (libcurl).
- Persistencia en archivos binarios de tamaño fijo.

---

## Requisitos

Para compilar y ejecutar el proyecto se necesitan:

| Dependencia | Para qué |
|---|---|
| `gcc` y `make` | Compilar el proyecto |
| SDL2 | Ventana y renderizado de la interfaz gráfica |
| SDL2_ttf | Renderizado de texto (fuentes) en la interfaz gráfica |
| libcurl | Envío de correos de verificación por SMTP |
| ca-certificates | Certificados para la conexión SSL del SMTP |

> La versión de **consola** también depende de libcurl, porque comparte la lógica
> de negocio (incluido el envío de correo) con la versión gráfica.

---

## Instalación de dependencias

### Linux (Ubuntu / Debian / WSL)

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev libsdl2-ttf-dev libcurl4-openssl-dev ca-certificates
sudo update-ca-certificates
```


### macOS

Con [Homebrew](https://brew.sh):

```bash
brew install sdl2 sdl2_ttf curl
```

El compilador (`clang`, compatible con la invocación a `gcc`) y `make` vienen con
las Command Line Tools de Xcode:

```bash
xcode-select --install
```

### Windows

La forma recomendada y más sencilla es usar **WSL2** (Windows Subsystem for Linux)
con Ubuntu, y seguir las instrucciones de Linux de arriba:

```powershell
# En PowerShell como administrador:
wsl --install
```

Luego se abre Ubuntu desde el menú de inicio y se siguen los pasos de la sección Linux.

> **Nota para WSL:** la interfaz gráfica funciona a través de WSLg (incluido en
> Windows 11 y en Windows 10 actualizado). Ver la sección *Solución de problemas*
> si la ventana aparece vacía.

Alternativamente, para compilar de forma nativa en Windows se puede usar **MSYS2**
(con el toolchain MinGW-w64) instalando los paquetes equivalentes:
`mingw-w64-x86_64-gcc`, `mingw-w64-x86_64-SDL2`, `mingw-w64-x86_64-SDL2_ttf`
y `mingw-w64-x86_64-curl`.

---

## Configuración del correo (SMTP)

El envío de los códigos de verificación requiere las credenciales de una cuenta
de correo emisora. Por seguridad, **estas credenciales no se incluyen en el
repositorio**: hay que crearlas localmente.

1. Copiar la plantilla de configuración:

    ```bash
    cp config/smtp.conf.example config/smtp.conf
    ```

2. Editar `config/smtp.conf` con las credenciales reales:

    ```
    smtp_servidor=smtp.gmail.com
    smtp_puerto=465
    smtp_usuario=tu_correo@gmail.com
    smtp_password=tu_app_password_de_16_caracteres
    ```

> Para Gmail se necesita una **contraseña de aplicación** (App Password) de 16
> caracteres, no la contraseña normal de la cuenta. Requiere tener activada la
> verificación en dos pasos.

El archivo `config/smtp.conf` está incluido en `.gitignore` y no debe subirse al
repositorio. Si el archivo no existe, el programa funciona igual pero el envío de
correos queda deshabilitado.

---

## Compilación y ejecución

Desde el directorio `src/`:

```bash
make             # compila ambas versiones (gráfica y consola)
make run         # compila (si hace falta) y ejecuta la versión gráfica
make run-consola # compila (si hace falta) y ejecuta la versión de consola
make clean       # elimina los ejecutables
```

Ambas versiones comparten los mismos archivos de datos, por lo que un cliente
creado en una interfaz es visible desde la otra.

### Administrador inicial

La primera vez que se ejecuta, el sistema crea un administrador por defecto:

- **Usuario:** `admin`
- **Contraseña:** `admin1234`

---

## Estructura del proyecto

```
src/
├── logica/      Lógica de negocio (cliente, cuenta, operaciones, login, admin…)
├── config/      Lectura de configuración SMTP (smtp.conf)
├── correo/      Envío de correos con libcurl
├── ui/          Interfaz gráfica SDL2
│   └── pantallas/   Cada pantalla de la aplicación
├── datos/       Archivos binarios de datos (generados, no versionados)
├── assets/      Fuentes y recursos
├── main_grafico.c   Punto de entrada de la versión gráfica
├── main_consola.c   Punto de entrada de la versión de consola
└── Makefile
```

---

## Solución de problemas

**La ventana gráfica aparece vacía o en negro (frecuente en WSL).**
El entorno gráfico puede no soportar el renderizado acelerado. En `ui/ventana.c`,
el renderer puede forzarse a modo software cambiando `SDL_RENDERER_ACCELERATED`
por `SDL_RENDERER_SOFTWARE`. Reiniciar WSL desde PowerShell también suele ayudar:

```powershell
wsl --shutdown
```

**Error al cargar fuente.**
Verificar que el programa se ejecute desde el directorio `src/`, donde la ruta a
`assets/fonts/` es válida.

**Error de certificado SSL al enviar correo.**
Instalar y actualizar los certificados del sistema:

```bash
sudo apt install ca-certificates
sudo update-ca-certificates
```

**Login denied al enviar correo.**
Revisar que `smtp.conf` use una contraseña de aplicación válida (no la contraseña
normal de la cuenta) y que la verificación en dos pasos esté activada.
