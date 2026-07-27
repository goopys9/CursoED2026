#include "terminal.h"

int main() {
    int opcion;

    inicializarSistema();

    do {
        mostrarMenu();
        printf("Seleccione una opcion: ");
        if (scanf("%d", &opcion) != 1) {
            limpiarEntrada();
            opcion = -1;
        }
        limpiarEntrada();

        switch (opcion) {
            case 1:
                registrarDestino();
                break;
            case 2:
                registrarPasajero();
                break;
            case 3:
                mostrarPasajerosPorDestino();
                break;
            case 4:
                registrarViaje();
                break;
            case 5:
                buscarViaje();
                break;
            case 6:
                mostrarArbolViajes();
                break;
            case 7:
                realizarEmbarque();
                break;
            case 8:
                consultarPasajero();
                break;
            case 9:
                mostrarTodosDestinos();
                break;
            case 10:
                mostrarEstadisticas();
                break;
            case 0:
                printf("Saliendo del sistema.\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while (opcion != 0);

    liberarSistema();
    return 0;
}