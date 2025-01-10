#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int num) {
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

int leerEnteroPositivo(const char *mensaje) {
    int valor;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &valor) != 1 || valor < 0 || getchar() != '\n') {
            printf("Error: Solo se permiten números enteros positivos.\n");
            while (getchar() != '\n');
        } else {
            return valor;
        }
    }
}

float leerFloatPositivo(const char *mensaje) {
    float valor;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &valor) != 1 || valor < 0 || getchar() != '\n') {
            printf("Error: Solo se permiten números positivos.\n");
            while (getchar() != '\n');
        } else {
            return valor;
        }
    }
}

void saveFactura(struct Factura *factura) {
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fwrite(factura, sizeof(struct Factura), 1, file);
    fwrite(factura->productos, sizeof(struct Producto), factura->numProductos, file);

    fclose(file);
    printf("Factura #%d guardada correctamente.\n", factura->numeroFactura);
}

void createFactura(int *contadorFactura) {
    struct Factura factura;
    factura.numeroFactura = ++(*contadorFactura);

    printf("Nombre del cliente: ");
    getchar();
    leerCadena(factura.nombre, 20);

    factura.cedula = leerEnteroPositivo("Cédula del cliente: ");
    factura.numProductos = leerEnteroPositivo("Número de productos: ");

    factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Producto %d:\n", i + 1);
        printf("  Nombre: ");
        getchar();
        leerCadena(factura.productos[i].nombre, 50);
        factura.productos[i].precio = leerFloatPositivo("  Precio: ");
        factura.productos[i].cantidad = leerEnteroPositivo("  Cantidad: ");
    }

    calcularTotalFactura(&factura, 12, 0); // 12% impuestos, 0% descuento
    imprimirFactura(&factura);

    saveFactura(&factura);
    free(factura.productos);
}

void readFactura() {
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL) {
        printf("No hay facturas registradas.\n");
        return;
    }

    struct Factura factura;
    struct Producto *productos = NULL;

    fseek(file, -sizeof(struct Factura), SEEK_END);
    fread(&factura, sizeof(struct Factura), 1, file);

    productos = malloc(factura.numProductos * sizeof(struct Producto));
    fseek(file, -((long)sizeof(struct Factura) + factura.numProductos * sizeof(struct Producto)), SEEK_END);
    fread(productos, sizeof(struct Producto), factura.numProductos, file);
    factura.productos = productos;

    imprimirFactura(&factura);

    free(productos);
    fclose(file);
}

void calcularTotalFactura(struct Factura *factura, float impuesto, float descuento) {
    factura->subtotal = 0;
    for (int i = 0; i < factura->numProductos; i++) {
        factura->subtotal += factura->productos[i].cantidad * factura->productos[i].precio;
    }
    factura->impuestos = factura->subtotal * (impuesto / 100);
    factura->descuentos = factura->subtotal * (descuento / 100);
    factura->total = factura->subtotal + factura->impuestos - factura->descuentos;
}

void imprimirFactura(struct Factura *factura) {
    printf("\n------------------------------------\n");
    printf("             FACTURA #%d\n", factura->numeroFactura);
    printf("------------------------------------\n");
    printf("Cliente: %-20s Cédula: %-10d\n", factura->nombre, factura->cedula);
    printf("------------------------------------\n");
    printf("%-15s %-10s %-15s %-10s\n", "Producto", "Cantidad", "Precio Unitario", "Monto");
    for (int i = 0; i < factura->numProductos; i++) {
        float monto = factura->productos[i].cantidad * factura->productos[i].precio;
        printf("%-15s %-10d %-15.2f %-10.2f\n", factura->productos[i].nombre, factura->productos[i].cantidad, factura->productos[i].precio, monto);
    }
    printf("------------------------------------\n");
    printf("Subtotal: %.2f\n", factura->subtotal);
    printf("Impuestos: %.2f\n", factura->impuestos);
    printf("Descuentos: %.2f\n", factura->descuentos);
    printf("Total: %.2f\n", factura->total);
    printf("------------------------------------\n\n");
}
