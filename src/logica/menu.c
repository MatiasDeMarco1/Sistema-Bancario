#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "cliente.h"
#include "cuenta.h"
#include "validaciones.h"
#include "login.h"
#include "operaciones.h"
#include "historial.h"

// ---- MENU INICIAL ----

void menuInicial() {
    int opcion;
    do {
        printf("\n=== SISTEMA BANCARIO ===\n");
        printf("1. Iniciar sesion\n");
        printf("2. Registrarse\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1: {
                Cliente cliente;
                if (login(&cliente)) {
                    if (!cliente.activo) {
                        printf("Tu cuenta esta dada de baja.\n");
                    } else {
                        menuPostLogin(&cliente);
                    }
                } else {
                    printf("Usuario o contrasena incorrectos.\n");
                }
                break;
            }
            case 2: {
                Cliente cliente;
                crearCliente(&cliente);
                printf("Registro exitoso. Ya podes iniciar sesion.\n");
                break;
            }
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
}

// ---- MENU POST LOGIN ----

void menuPostLogin(Cliente *cliente) {
    int tienePesos   = clienteTieneCuenta(cliente->cuit, PESOS);
    int tieneDolares = clienteTieneCuenta(cliente->cuit, DOLARES);
    int opcion;

    do {
        printf("\nBienvenido, %s %s\n", cliente->nombre, cliente->apellido);
        printf("--- MIS CUENTAS ---\n");

        if (!tienePesos)   printf("1. Crear cuenta en Pesos\n");
        if (!tieneDolares) printf("2. Crear cuenta en Dolares\n");
        if (tienePesos || tieneDolares) printf("3. Seleccionar cuenta\n");
        printf("9. Darme de baja\n");
        printf("0. Cerrar sesion\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                if (!tienePesos) {
                    Cuenta c;
                    crearCuenta(&c, cliente->cuit, PESOS);
                    tienePesos = 1;
                } else {
                    printf("Ya tenes cuenta en pesos.\n");
                }
                break;
            case 2:
                if (!tieneDolares) {
                    Cuenta c;
                    crearCuenta(&c, cliente->cuit, DOLARES);
                    tieneDolares = 1;
                } else {
                    printf("Ya tenes cuenta en dolares.\n");
                }
                break;
            case 3:
                if (tienePesos || tieneDolares)
                    menuSeleccionCuenta(cliente);
                else
                    printf("No tenes cuentas aun.\n");
                break;
            case 9: {
                char confirm[5];
                printf("Estas seguro que queres darte de baja? (s/n): ");
                fgets(confirm, sizeof(confirm), stdin);
                if (confirm[0] == 's' || confirm[0] == 'S') {
                    eliminarCliente(cliente->cuit);
                    printf("Te diste de baja exitosamente.\n");
                    return; 
                }
                break;
            }
            case 0:
                printf("Sesion cerrada.\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
}

// ---- SELECCION DE CUENTA ----

void menuSeleccionCuenta(Cliente *cliente) {
    int opcion = 0;
    int tienePesos   = clienteTieneCuenta(cliente->cuit, PESOS);
    int tieneDolares = clienteTieneCuenta(cliente->cuit, DOLARES);

    printf("\n--- SELECCIONAR CUENTA ---\n");
    if (tienePesos)   printf("1. Cuenta en Pesos\n");
    if (tieneDolares) printf("2. Cuenta en Dolares\n");
    printf("0. Volver\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar();

    Cuenta cuenta;
    int encontrada = 0;

    FILE *f = fopen("./datos/cuentas.dat", "rb");
    if (f == NULL) { printf("Error al abrir archivo.\n"); return; }

    while (fread(&cuenta, sizeof(Cuenta), 1, f)) {
        if (strcmp(cuenta.cliente_cuit, cliente->cuit) == 0 && cuenta.activa) {
            if (opcion == 1 && cuenta.moneda == PESOS)   { encontrada = 1; break; }
            if (opcion == 2 && cuenta.moneda == DOLARES) { encontrada = 1; break; }
        }
    }
    fclose(f);

    if (encontrada)
        menuCuenta(&cuenta);
    else if (opcion != 0)
        printf("Cuenta no encontrada.\n");
}

// ---- MENU DE CUENTA ----

void menuCuenta(Cuenta *cuenta) {
    int opcion;
    do {
        printf("\n--- CUENTA %s ---\n", cuenta->moneda == PESOS ? "PESOS" : "DOLARES");
        printf("CBU:    %s\n",   cuenta->cbu);
        printf("Alias:  %s\n",   cuenta->alias);
        printf("Saldo:  %.2f\n", cuenta->saldo);
        printf("\n");
        printf("1. Ingresar dinero\n");
        printf("2. Transferir\n");
        printf("3. Retirar\n");
        printf("4. Modificar alias\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
        case 1:
            ingresarDinero(cuenta);
        break;
        case 2:
            transferir(cuenta, cuenta->cliente_cuit);
        break;
        case 3:
            retirarDinero(cuenta);
        break;
        case 4:
            editarAlias(cuenta);
            break;
        case 0:
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 0);
}

