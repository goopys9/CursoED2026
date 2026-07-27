#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE 30

typedef enum {
    TIPO_CC = 1,
    TIPO_PASAPORTE = 2,
    TIPO_TI = 3
} TipoDocumento;

typedef enum {
    ESTADO_ESPERA = 0,
    ESTADO_EMBARCADO = 1
} EstadoPasajero;

typedef struct NodoPasajero {
    int documento;
    TipoDocumento tipoDocumento;
    EstadoPasajero estado;
    struct NodoPasajero *siguiente;
} NodoPasajero;

typedef struct ColaPasajeros {
    NodoPasajero *frente;
    NodoPasajero *final;
    int cantidad;
} ColaPasajeros;

typedef struct NodoViaje {
    int codigo;
    int capacidad;
    int altura;
    int pasajerosEmbarcados;
    struct NodoViaje *izq;
    struct NodoViaje *der;
} NodoViaje;

typedef struct Destino {
    int codigo;
    char nombre[MAX_NOMBRE];
    char empresa[MAX_NOMBRE];
    ColaPasajeros cola;
    NodoViaje *raizViajes;
    struct Destino *siguiente;
} Destino;

extern Destino *inicioDestinos;
extern int totalPasajerosRegistrados;
extern int totalPasajerosEmbarcados;
extern int totalDestinos;

void inicializarSistema();
void mostrarMenu();
void limpiarEntrada();
void registrarDestino();
void registrarPasajero();
void mostrarPasajerosPorDestino();
void registrarViaje();
void buscarViaje();
void mostrarArbolViajes();
void realizarEmbarque();
void consultarPasajero();
void mostrarTodosDestinos();
void mostrarEstadisticas();
void liberarSistema();

Destino *buscarDestinoPorCodigo(int codigo);
void agregarDestino(Destino *nuevoDestino);
void mostrarDestino(Destino *destino);
void mostrarColaPasajeros(ColaPasajeros *cola);
int contarPasajerosEspera(ColaPasajeros *cola);
int verificarColaVacia(ColaPasajeros *cola);
void encolarPasajero(ColaPasajeros *cola, NodoPasajero *nuevo);
NodoPasajero *desencolarPasajero(ColaPasajeros *cola);
NodoPasajero *consultarPrimerPasajero(ColaPasajeros *cola);
void liberarColaPasajeros(ColaPasajeros *cola);

NodoViaje *insertarViajeEnArbol(NodoViaje *raiz, int codigo, int capacidad);
NodoViaje *buscarViajeEnArbol(NodoViaje *raiz, int codigo);
void mostrarViajesEnOrden(NodoViaje *raiz);
void liberarArbolViajes(NodoViaje *raiz);

#endif