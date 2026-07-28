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

// Muestra la información de un destino
void mostrarDestino(Destino *destino) {
    // Verifica que el destino exista
    if (destino == NULL) {
        return;
    }

    // Imprime los datos del destino
    printf("Codigo: %d\n", destino->codigo);
    printf("Nombre: %s\n", destino->nombre);
    printf("Empresa: %s\n", destino->empresa);
    printf("Pasajeros en espera: %d\n", destino->cola.cantidad);

    // Verifica si el destino tiene viajes registrados
    printf("Viajes registrados: ");
    if (destino->raizViajes == NULL) {
        printf("No hay viajes\n");
    } else {
        printf("Si\n");
    }

    // Línea separadora
    printf("-------------------------------\n");
}

// Muestra todos los destinos registrados
void mostrarTodosDestinos() {
    Destino *actual = inicioDestinos;

    // Verifica si no hay destinos
    if (actual == NULL) {
        printf("No hay destinos registrados.\n");
        return;
    }

    printf("\nDestinos registrados:\n");

    // Recorre la lista de destinos y los muestra
    while (actual != NULL) {
        mostrarDestino(actual);
        actual = actual->siguiente;
    }
}

// Libera toda la memoria utilizada por el sistema
void liberarSistema() {
    Destino *actual = inicioDestinos;

    // Recorre la lista de destinos
    while (actual != NULL) {
        // Guarda el siguiente destino antes de liberar el actual
        Destino *siguiente = actual->siguiente;

        // Libera la cola de pasajeros
        liberarColaPasajeros(&actual->cola);

        // Libera el árbol de viajes
        liberarArbolViajes(actual->raizViajes);

        // Libera la memoria del destino
        free(actual);

        // Continúa con el siguiente destino
        actual = siguiente;
    }

    // Reinicia las variables globales
    inicioDestinos = NULL;
    totalPasajerosRegistrados = 0;
    totalPasajerosEmbarcados = 0;
    totalDestinos = 0;
}

// Calcula y muestra las estadísticas del sistema
void mostrarEstadisticas() {
    // Empieza el recorrido desde el primer destino
    Destino *actual = inicioDestinos;

    // Variables para almacenar las estadísticas
    int totalEspera = 0;
    int totalViajes = 0;
    int totalEmbarcadosPorViaje = 0;

    // Guardan el destino con más y menos pasajeros en espera
    Destino *destinoMayor = NULL;
    Destino *destinoMenor = NULL;

    // Variables para comparar cantidades
    int mayorEspera = -1;
    int menorEspera = -1;

    // Verifica si existe al menos un destino para analizar
    if (actual == NULL) {
        // Si no hay datos, se informa al usuario y se termina la función
        printf("No hay datos para mostrar.\n");
        return;
    }

    // Recorre todos los destinos para acumular las estadísticas generales
    while (actual != NULL) {
        // Suma la cantidad de pasajeros en espera del destino actual
        totalEspera += actual->cola.cantidad;

        // Busca el destino con mayor cantidad de pasajeros en espera
        if (mayorEspera == -1 || actual->cola.cantidad > mayorEspera) {
            mayorEspera = actual->cola.cantidad;
            destinoMayor = actual;
        }

        // Busca el destino con menor cantidad de pasajeros en espera
        if (menorEspera == -1 || actual->cola.cantidad < menorEspera) {
            menorEspera = actual->cola.cantidad;
            destinoMenor = actual;
        }

        // Recorre los viajes del árbol de cada destino para sumar el total de viajes
        NodoViaje *raiz = actual->raizViajes;
        while (raiz != NULL) {
            totalViajes++;
            totalEmbarcadosPorViaje += raiz->pasajerosEmbarcados;
            raiz = raiz->der;
        }

        // Continúa con el siguiente destino de la lista
        actual = actual->siguiente;
    }

    // Muestra las estadísticas acumuladas en pantalla
    printf("\n==== ESTADISTICAS ====\n");
    printf("Total de destinos: %d\n", totalDestinos);
    printf("Total de pasajeros registrados: %d\n", totalPasajerosRegistrados);
    printf("Total de pasajeros embarcados: %d\n", totalPasajerosEmbarcados);
    if (totalDestinos > 0) {
        printf("Promedio de pasajeros en espera por destino: %.2f\n", (float) totalEspera / totalDestinos);
    } else {
        printf("Promedio de pasajeros en espera por destino: 0.00\n");
    }
    if (totalViajes > 0) {
        printf("Promedio de pasajeros embarcados por viaje: %.2f\n", (float) totalEmbarcadosPorViaje / totalViajes);
    } else {
        printf("Promedio de pasajeros embarcados por viaje: 0.00\n");
    }
    if (destinoMayor != NULL) {
        printf("Destino con mayor espera: %s (%d)\n", destinoMayor->nombre, mayorEspera);
    }
    if (destinoMenor != NULL) {
        printf("Destino con menor espera: %s (%d)\n", destinoMenor->nombre, menorEspera);
    }
}