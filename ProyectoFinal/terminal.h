#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 30  // Tamaño máximo para nombres o cadenas relacionadas

typedef enum {
    TIPO_CC = 1,        // Cédula de ciudadanía
    TIPO_PASAPORTE = 2, // Pasaporte
    TIPO_TI = 3         // Tarjeta de identidad
} TipoDocumento;

typedef enum {
    ESTADO_ESPERA = 0,   // El pasajero está en espera
    ESTADO_EMBARCADO = 1 // El pasajero ya abordó el vuelo
} EstadoPasajero;

typedef struct NodoPasajero {
    int documento;                 // Número de documento del pasajero
    TipoDocumento tipoDocumento;   // Tipo de documento del pasajero
    EstadoPasajero estado;         // Estado actual del pasajero
    struct NodoPasajero *siguiente; // Puntero al siguiente pasajero en la cola
} NodoPasajero;

typedef struct ColaPasajeros {
    NodoPasajero *frente; // Primer pasajero de la cola
    NodoPasajero *final;  // Último pasajero de la cola
    int cantidad;        // Número total de pasajeros en la cola
} ColaPasajeros;
// Nodo del árbol binario que almacena la información de un viaje
typedef struct NodoViaje {
    int codigo;                  // Código único del viaje
    int capacidad;               // Capacidad máxima de pasajeros
    int altura;                  // Altura del nodo (usada si el árbol es AVL)
    int pasajerosEmbarcados;     // Cantidad de pasajeros que ya embarcaron

    struct NodoViaje *izq;        // Puntero al hijo izquierdo
    struct NodoViaje *der;        // Puntero al hijo derecho
} NodoViaje;


// Estructura que representa un destino
typedef struct Destino {
    int codigo;                  // Código identificador del destino
    char nombre[MAX_NOMBRE];     // Nombre del destino
    char empresa[MAX_NOMBRE];    // Empresa encargada del destino

    ColaPasajeros cola;          // Cola de pasajeros que esperan embarcar

    NodoViaje *raizViajes;       // Raíz del árbol de viajes asociados al destino

    struct Destino *siguiente;   // Puntero al siguiente destino (lista enlazada)
} Destino;


// Variables globales del sistema

extern Destino *inicioDestinos;          // Inicio de la lista de destinos

extern int totalPasajerosRegistrados;    // Total de pasajeros registrados

extern int totalPasajerosEmbarcados;     // Total de pasajeros que ya embarcaron

extern int totalDestinos;                // Número total de destinos registrados


// ===== Prototipos de funciones =====

// Inicializa todas las estructuras y variables del sistema
void inicializarSistema();

// Muestra el menú principal al usuario
void mostrarMenu();

// Limpia el buffer de entrada del teclado
void limpiarEntrada();

// Registra un nuevo destino
void registrarDestino();

// Registra un nuevo pasajero en un destino
void registrarPasajero();

// Muestra los pasajeros registrados de un destino
void mostrarPasajerosPorDestino();

// Registra un nuevo viaje dentro del árbol del destino
void registrarViaje();

// Busca un viaje por su código
void buscarViaje();

// Muestra el árbol de viajes (por ejemplo, en recorrido inorden)
void mostrarArbolViajes();

// Realiza el embarque de pasajeros en un viaje
void realizarEmbarque();

// Consulta la información de un pasajero específico
void consultarPasajero();

// Muestra todos los destinos registrados
void mostrarTodosDestinos();

// Muestra estadísticas generales del sistema
void mostrarEstadisticas();

// Libera toda la memoria dinámica utilizada por el sistema
void liberarSistema(); 
// Busca un destino por su código y devuelve un puntero al destino encontrado.
// Si no existe, normalmente devuelve NULL.
Destino *buscarDestinoPorCodigo(int codigo);

// Agrega un nuevo destino a la estructura donde se almacenan los destinos.
void agregarDestino(Destino *nuevoDestino);

// Muestra en pantalla la información de un destino.
void mostrarDestino(Destino *destino);

// Muestra todos los pasajeros que se encuentran en la cola de espera.
void mostrarColaPasajeros(ColaPasajeros *cola);

// Cuenta y devuelve la cantidad de pasajeros que hay en la cola.
int contarPasajerosEspera(ColaPasajeros *cola);

// Verifica si la cola está vacía.
// Devuelve 1 si está vacía y 0 si contiene pasajeros (según la implementación).
int verificarColaVacia(ColaPasajeros *cola);

// Agrega un nuevo pasajero al final de la cola (encolar).
void encolarPasajero(ColaPasajeros *cola, NodoPasajero *nuevo);

// Elimina y devuelve el primer pasajero de la cola (desencolar).
// Si la cola está vacía, normalmente devuelve NULL.
NodoPasajero *desencolarPasajero(ColaPasajeros *cola);

// Devuelve el primer pasajero de la cola sin eliminarlo.
// Si la cola está vacía, devuelve NULL.
NodoPasajero *consultarPrimerPasajero(ColaPasajeros *cola);

// Libera toda la memoria utilizada por la cola de pasajeros.
void liberarColaPasajeros(ColaPasajeros *cola);

// Inserta un nuevo viaje en el árbol binario de búsqueda utilizando su código.
// También almacena la capacidad del viaje.
// Devuelve la nueva raíz del árbol.
NodoViaje *insertarViajeEnArbol(NodoViaje *raiz, int codigo, int capacidad);

// Busca un viaje por su código dentro del árbol.
// Devuelve un puntero al viaje si existe o NULL en caso contrario.
NodoViaje *buscarViajeEnArbol(NodoViaje *raiz, int codigo);

// Recorre el árbol en orden (inorden) y muestra todos los viajes.
void mostrarViajesEnOrden(NodoViaje *raiz);

// Libera toda la memoria ocupada por el árbol de viajes.
void liberarArbolViajes(NodoViaje *raiz);

#endif  // Fin del archivo de cabecera (header guard)