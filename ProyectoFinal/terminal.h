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