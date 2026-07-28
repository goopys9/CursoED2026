#include "terminal.h"   // Librería donde están las estructuras y los prototipos de funciones.

// Puntero al inicio de la lista enlazada de destinos.
Destino *inicioDestinos = NULL;

// Variables globales para llevar el control de las estadísticas.
int totalPasajerosRegistrados = 0;
int totalPasajerosEmbarcados = 0;
int totalDestinos = 0;

// Inicializa todas las variables del sistema.
void inicializarSistema() {
    inicioDestinos = NULL;                  // No hay destinos registrados.
    totalPasajerosRegistrados = 0;          // Reinicia el contador de pasajeros.
    totalPasajerosEmbarcados = 0;           // Reinicia el contador de embarques.
    totalDestinos = 0;                      // Reinicia el contador de destinos.
}

// Limpia el búfer del teclado para evitar errores en futuras entradas.
void limpiarEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Sigue leyendo caracteres hasta encontrar un salto de línea.
    }
}

// Muestra el menú principal del programa.
void mostrarMenu() {
    printf("\n===== TERMINAL PORTUARIO TURISTICO =====\n");
    printf("1. Registrar destino\n");
    printf("2. Registrar pasajero\n");
    printf("3. Mostrar pasajeros por destino\n");
    printf("4. Registrar viaje programado\n");
    printf("5. Buscar viaje\n");
    printf("6. Mostrar arbol de viajes\n");
    printf("7. Realizar embarque\n");
    printf("8. Consultar pasajero\n");
    printf("9. Mostrar destinos registrados\n");
    printf("10. Mostrar estadisticas\n");
    printf("0. Salir\n");
    printf("======================================\n");
}

// Busca un destino usando su código.
Destino *buscarDestinoPorCodigo(int codigo) {

    // Empieza el recorrido desde el primer destino.
    Destino *actual = inicioDestinos;

    // Recorre toda la lista hasta encontrar el destino o llegar al final.
    while (actual != NULL) {

        // Si el código coincide, retorna el destino encontrado.
        if (actual->codigo == codigo) {
            return actual;
        }

        // Avanza al siguiente destino de la lista.
        actual = actual->siguiente;
    }

    // Si no encontró el destino, retorna NULL.
    return NULL;
}

// Agrega un nuevo destino al inicio de la lista enlazada.
void agregarDestino(Destino *nuevoDestino) {

    // El nuevo destino apunta al que antes era el primero.
    nuevoDestino->siguiente = inicioDestinos;

    // Ahora el nuevo destino pasa a ser el primero de la lista.
    inicioDestinos = nuevoDestino;

    // Incrementa el número total de destinos registrados.
    totalDestinos++;
}
