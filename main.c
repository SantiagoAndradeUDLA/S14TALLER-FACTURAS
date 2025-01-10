#include <stdio.h>
#include "funciones.h"

int menu() {
    int opcion;
    printf("\n--- MENU PRINCIPAL ---\n");
    printf("1. Crear factura\n");
    printf("2. Leer última factura\n");
    printf("3. Salir\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    return opcion;
}

int main() {
    int opcion;
    int contadorFactura = 0;

    do {
        opcion = menu();
        switch (opcion) {
        case 1:
            createFactura(&contadorFactura);
            break;
        case 2:
            readFactura();
            break;
        case 3:
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opción inválida.\n");
        }
    } while (opcion != 3);

    return 0;
}
