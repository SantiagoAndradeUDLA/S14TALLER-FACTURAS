#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

void leerCadena(char *cadena, int num)
{
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (cadena[len] == '\n')
        cadena[len] = '\0';
}

int leerEnteroPositivo(const char *mensaje)
{
    int valor;
    while (1)
    {
        printf("%s", mensaje);
        if (scanf("%d", &valor) != 1 || valor < 0 || getchar() != '\n')
        {
            printf("Error: Solo se permiten numeros enteros positivos.\n");
            while (getchar() != '\n');
        }
        else
        {
            return valor;
        }
    }
}

float leerFloatPositivo(const char *mensaje)
{
    float valor;
    while (1)
    {
        printf("%s", mensaje);
        if (scanf("%f", &valor) != 1 || valor < 0 || getchar() != '\n')
        {
            printf("Error: Solo se permiten numeros positivos.\n");
            while (getchar() != '\n');
        }
        else
        {
            return valor;
        }
    }
}

void validarCantidadEntera(struct Producto *producto)
{
    float cantidad;
    do
    {
        cantidad = leerFloatPositivo("  Cantidad: ");
        if ((int)cantidad != cantidad)
        {
            printf("Error: La cantidad debe ser un numero entero.\n");
        }
    } while ((int)cantidad != cantidad);
    producto->cantidad = (int)cantidad;
}

void calcularTotalFactura(struct Factura *factura, float impuesto, float descuento)
{
    factura->total = 0;
    float subtotal = 0;
    for (int i = 0; i < factura->numProductos; i++)
    {
        subtotal += factura->productos[i].cantidad * factura->productos[i].precio;
    }
    float impuestos = subtotal * (impuesto / 100);
    float descuentos = subtotal * (descuento / 100);
    factura->total = subtotal + impuestos - descuentos;
    factura->subtotal = subtotal;
    factura->impuestos = impuestos;
    factura->descuentos = descuentos;
}

void imprimirFactura(struct Factura *factura)
{
    printf("\n------------------------------------\n");
    printf("             FACTURA #%d\n", factura->numeroFactura);
    printf("------------------------------------\n");
    printf("Cliente: %-20s Cedula: %-10d\n", factura->nombre, factura->cedula);
    printf("------------------------------------\n");
    printf("%-15s %-10s %-15s %-10s\n", "Producto", "Cantidad", "Precio Unitario", "Monto");
    for (int i = 0; i < factura->numProductos; i++)
    {
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

void saveFactura(struct Factura *factura)
{
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }
    fwrite(factura, sizeof(struct Factura), 1, file);
    fwrite(factura->productos, sizeof(struct Producto), factura->numProductos, file);
    fclose(file);
}

void createFactura(int *contadorFactura)
{
    struct Factura factura;
    factura.numeroFactura = ++(*contadorFactura);

    printf("Nombre del cliente: ");
    getchar();  // Limpia el buffer antes de leer el nombre
    leerCadena(factura.nombre, 20);

    factura.cedula = leerEnteroPositivo("Cedula del cliente: ");
    factura.numProductos = leerEnteroPositivo("Numero de productos: ");

    factura.productos = malloc(factura.numProductos * sizeof(struct Producto));

    for (int i = 0; i < factura.numProductos; i++)
    {
        printf("Producto #%d\n", i + 1);
        printf("  Nombre: ");
        getchar();  // Limpia el buffer antes de leer el nombre del producto
        leerCadena(factura.productos[i].nombre, 50);
        validarCantidadEntera(&factura.productos[i]);
        factura.productos[i].precio = leerFloatPositivo("  Precio: ");
    }

    float impuesto = leerFloatPositivo("Ingrese el porcentaje de impuesto: ");
    float descuento = leerFloatPositivo("Ingrese el porcentaje de descuento: ");

    calcularTotalFactura(&factura, impuesto, descuento);
    saveFactura(&factura);
    imprimirFactura(&factura);

    free(factura.productos);
}

void readFactura()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    struct Factura factura;
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = (struct Producto *)malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);
        imprimirFactura(&factura);
        free(factura.productos);
    }

    fclose(file);
}

void buscarFactura()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    int opcion = leerEnteroPositivo("\nBuscar por:\n1. Numero de factura\n2. Cedula\n3. Nombre del cliente\nOpcion: ");
    struct Factura factura;
    char nombre[20];
    int cedula, numeroFactura, encontrado = 0;

    // Corrección en la captura del nombre
    if (opcion == 1)
    {
        numeroFactura = leerEnteroPositivo("Ingrese el numero de factura: ");
    }
    else if (opcion == 2)
    {
        cedula = leerEnteroPositivo("Ingrese la cedula: ");
    }
    else if (opcion == 3)
    {
        printf("Ingrese el nombre del cliente: ");
        getchar();  // Limpia el buffer antes de leer el nombre
        leerCadena(nombre, 20);
    }
    else
    {
        printf("Opcion invalida.\n");
        fclose(file);
        return;
    }
     while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        // Corrección en la comparación de nombres
        if ((opcion == 1 && factura.numeroFactura == numeroFactura) ||
            (opcion == 2 && factura.cedula == cedula) ||
            (opcion == 3 && strcmp(factura.nombre, nombre) == 0))
        {
            imprimirFactura(&factura);
            encontrado = 1;
        }

        free(factura.productos);
    }

    if (!encontrado)
    {
        printf("No se encontro una factura con los datos proporcionados.\n");
    }

    fclose(file);
}


void editarFactura()
{
    FILE *file = fopen("factura.dat", "rb+");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    int numeroFactura = leerEnteroPositivo("Ingrese el numero de factura a editar: ");
    struct Factura factura;
    int encontrado = 0;
    long posicion;

    while ((posicion = ftell(file)) >= 0 && fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        if (factura.numeroFactura == numeroFactura)
        {
            encontrado = 1;
            printf("Factura encontrada.\n");
            imprimirFactura(&factura);

            printf("Seleccione una opcion para editar:\n");
            printf("1. Actualizar numero de factura\n");
            printf("2. Actualizar nombre\n");
            printf("3. Actualizar cedula\n");
            printf("4. Actualizar numero de productos\n");
            printf("5. Actualizar precio\n");
            printf("6. Actualizar descuento\n");
            printf("7. Eliminar factura\n");
            printf("8. Salir\n");
            printf("Opcion: ");

            int opcion;
            scanf("%d", &opcion);

            switch (opcion)
            {
                case 1:
                    factura.numeroFactura = leerEnteroPositivo("Ingrese el nuevo numero de factura: ");
                    break;
                case 2:
                    printf("Ingrese el nuevo nombre: ");
                    getchar();
                    leerCadena(factura.nombre, 20);
                    break;
                case 3:
                    factura.cedula = leerEnteroPositivo("Ingrese la nueva cedula: ");
                    break;
                case 4:
                    factura.numProductos = leerEnteroPositivo("Ingrese el nuevo numero de productos: ");
                    factura.productos = realloc(factura.productos, factura.numProductos * sizeof(struct Producto));
                    for (int i = 0; i < factura.numProductos; i++)
                    {
                        printf("Producto #%d\n", i + 1);
                        printf("  Nombre: ");
                        getchar();
                        leerCadena(factura.productos[i].nombre, 50);
                        validarCantidadEntera(&factura.productos[i]);
                        factura.productos[i].precio = leerFloatPositivo("  Precio: ");
                    }
                    break;
                case 5:
                    for (int i = 0; i < factura.numProductos; i++)
                    {
                        printf("Actualizar precio del producto #%d\n", i + 1);
                        factura.productos[i].precio = leerFloatPositivo("  Nuevo precio: ");
                    }
                    break;
                case 6:
                    printf("Ingrese el nuevo porcentaje de descuento: ");
                    float descuento;
                    descuento = leerFloatPositivo("  Nuevo descuento: ");
                    calcularTotalFactura(&factura, factura.impuestos, descuento);
                    break;
                case 7:
                    eliminarFactura();
                    return;
                case 8:
                    printf("Saliendo del menu de edicion.\n");
                    break;
                default:
                    printf("Opcion invalida.\n");
                    break;
            }

            calcularTotalFactura(&factura, factura.impuestos, factura.descuentos);

            fseek(file, posicion, SEEK_SET);
            fwrite(&factura, sizeof(struct Factura), 1, file);
            fwrite(factura.productos, sizeof(struct Producto), factura.numProductos, file);
        }

        free(factura.productos);
    }

    if (!encontrado)
    {
        printf("No se encontro la factura con el numero proporcionado.\n");
    }

    fclose(file);
}

void eliminarFactura() {
    FILE *file = fopen("factura.dat", "rb");
    if (!file) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    int numFacturas = fileSize > 0 ? fileSize / sizeof(struct Factura) : 0;
    struct Factura *facturas = numFacturas > 0 ? malloc(numFacturas * sizeof(struct Factura)) : NULL;

    if (numFacturas > 0 && !facturas) {
        printf("Error al asignar memoria.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < numFacturas; i++) {
        fread(&facturas[i], sizeof(struct Factura), 1, file);
        facturas[i].productos = malloc(facturas[i].numProductos * sizeof(struct Producto));
        if (!facturas[i].productos) {
            printf("Error al asignar memoria para productos.\n");
            fclose(file);
            for (int j = 0; j < i; j++) free(facturas[j].productos);
            free(facturas);
            return;
        }
        fread(facturas[i].productos, sizeof(struct Producto), facturas[i].numProductos, file);
    }

    fclose(file);

    int numEliminar = leerEnteroPositivo("Ingrese el numero de factura a eliminar: ");
    int nuevasNumFacturas = 0;

    for (int i = 0; i < numFacturas; i++) {
        if (facturas[i].numeroFactura == numEliminar) {
            printf("Factura #%d eliminada.\n", numEliminar);
            free(facturas[i].productos);
        } else {
            facturas[nuevasNumFacturas++] = facturas[i]; // Combinación de asignación e incremento
        }
    }

    if (nuevasNumFacturas == numFacturas) { // No se encontró la factura
        printf("No se encontro la factura.\n");
        for (int i = 0; i < numFacturas; i++) free(facturas[i].productos);
        free(facturas);
        return;
    }

    file = fopen("factura.dat", "wb");
    if (!file) {
        printf("Error al reabrir el archivo.\n");
        for (int i = 0; i < numFacturas; i++) free(facturas[i].productos);
        free(facturas);
        return;
    }

    for (int i = 0; i < nuevasNumFacturas; i++) {
        fwrite(&facturas[i], sizeof(struct Factura), 1, file);
        fwrite(facturas[i].productos, sizeof(struct Producto), facturas[i].numProductos, file);
        free(facturas[i].productos);
    }

    fclose(file);
    free(facturas);
}

void reporteResumen()
{
    FILE *file = fopen("factura.dat", "rb");
    if (file == NULL)
    {
        printf("Error al abrir el archivo\n");
        return;
    }

    float totalFacturado = 0;
    int totalFacturas = 0;
    char mejorCliente[20] = "";
    float mayorTotal = 0;

    struct Factura factura;
    while (fread(&factura, sizeof(struct Factura), 1, file))
    {
        factura.productos = malloc(factura.numProductos * sizeof(struct Producto));
        fread(factura.productos, sizeof(struct Producto), factura.numProductos, file);

        totalFacturado += factura.total;
        totalFacturas++;

        if (factura.total > mayorTotal)
        {
            mayorTotal = factura.total;
            strcpy(mejorCliente, factura.nombre);
        }

        free(factura.productos);
    }

    fclose(file);

    printf("\n--- RESUMEN ---\n");
    printf("Total facturado: %.2f\n", totalFacturado);
    printf("Numero total de facturas: %d\n", totalFacturas);
    printf("Mejor cliente: %s con un total de %.2f\n", mejorCliente, mayorTotal);
}
