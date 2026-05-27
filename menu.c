#include <stdio.h>
#include "menu.h"
#include "cliente.h"
#include "validaciones.h"

void menuClientes() {
    int opcion;

    do {
        printf("\n--- MENU CLIENTES ---\n");
        printf("1. Crear cliente\n");
        printf("2. Mostrar cliente por ID\n");
        printf("3. Editar cliente\n");
        printf("4. Eliminar cliente\n");
        printf("5. Mostrar todos\n");
        printf("0. Volver\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1: {
                Cliente c;
                crearCliente(&c);
                printf("Cliente creado con ID %d.\n", c.id);
                break;
            }
            case 2: {
                int id;
                Cliente c;
                printf("Ingrese ID: ");
                scanf("%d", &id);
                getchar();
                if (buscarCliente(id, &c))
                    mostrarCliente(&c);
                else
                    printf("Cliente no encontrado.\n");
                break;
            }
            case 3: {
                int id;
                Cliente c;
                printf("Ingrese ID a editar: ");
                scanf("%d", &id);
                getchar();
                if (buscarCliente(id, &c))
                    editarCliente(&c);
                else
                    printf("Cliente no encontrado.\n");
                break;
            }
            case 4: {
                int id;
                printf("Ingrese ID a eliminar: ");
                scanf("%d", &id);
                getchar();
                eliminarCliente(id);
                break;
            }
            case 5:
                mostrarTodos();
                break;
            case 0:
                break;
            default:
                printf("Opcion invalida.\n");
        }

    } while (opcion != 0);
}

void menuPrincipal() {
    int opcion;

    do {
        printf("\n=== SISTEMA BANCARIO ===\n");
        printf("1. Clientes\n");
        printf("2. Cuentas\n");
        printf("3. Transferencias\n");
        printf("4. Depositos y retiros\n");
        printf("5. Historial\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1: menuClientes();     break;
            case 2: /* menuCuentas() */ break;
            case 3: /* menuTransferencias() */ break;
            case 4: /* menuOperaciones() */ break;
            case 5: /* menuHistorial() */ break;
            case 0: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida.\n");
        }

    } while (opcion != 0);
}