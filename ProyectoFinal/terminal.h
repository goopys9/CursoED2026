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