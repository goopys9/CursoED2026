#include "terminal.h"

/* 
 * Valida si el tipo de documento ingresado cumple con las constantes permitidas.
 * Retorna 1 (verdadero) si es válido, 0 (falso) si no.
 */
static int documentoValido(int tipo) {
    return tipo == TIPO_CC || tipo == TIPO_PASAPORTE || tipo == TIPO_TI;
}

/* 
 * BÚSQUEDA GENERAL EN ESTRUCTURA ANIDADA:
 * Recorre una lista enlazada de 'Destinos' y, dentro de cada destino, 
 * recorre la cola de pasajeros para evitar duplicados en el sistema.
 * Complejidad: O(N * M) donde N = destinos y M = pasajeros por destino.
 */
static int existePasajeroEnSistema(int documento) {
    Destino *actual = inicioDestinos;
    
    // Recorremos la lista principal de destinos
    while (actual != NULL) {
        // Obtenemos el puntero al primer pasajero de la cola del destino actual
        NodoPasajero *aux = actual->cola.frente;
        
        // Recorremos la sub-lista (cola) de pasajeros de este destino
        while (aux != NULL) {
            if (aux->documento == documento) {
                return 1; // Pasajero encontrado (existe)
            }
            aux = aux->siguiente; // Avanzamos al siguiente nodo de la cola
        }
        actual = actual->siguiente; // Avanzamos al siguiente destino
    }
    return 0; // No se encontró el documento en ningún destino
}

/* 
 * Retorna la cantidad de pasajeros en espera.
 * Usa operador ternario para evitar errores de puntero nulo (NULL pointer dereference).
 */
int contarPasajerosEspera(ColaPasajeros *cola) {
    return cola != NULL ? cola->cantidad : 0;
}

/* 
 * Verifica si la cola está vacía o si el puntero a la cola es nulo.
 * Retorna 1 si está vacía/nula, 0 si tiene al menos un elemento.
 */
int verificarColaVacia(ColaPasajeros *cola) {
    return cola == NULL || cola->cantidad == 0;
}

/* 
 * OPERACIÓN ENCOLAR (Enqueue): Inserta un nodo al FINAL de la cola (Estructura FIFO).
 * Punteros involucrados: 'frente' (primer elemento) y 'final' (último elemento).
 */
void encolarPasajero(ColaPasajeros *cola, NodoPasajero *nuevo) {
    // Control de seguridad por si envían punteros nulos
    if (cola == NULL || nuevo == NULL) {
        return;
    }

    // Aseguramos que el nuevo nodo sea el último, por lo que su 'siguiente' debe ser NULL
    nuevo->siguiente = NULL;

    // CASO 1: Si la cola está vacía, el nuevo nodo es tanto el 'frente' como el 'final'
    if (cola->final == NULL) {
        cola->frente = nuevo;
        cola->final = nuevo;
    } 
    // CASO 2: Si ya hay elementos, enlazamos el 'final' actual con el 'nuevo' y actualizamos el puntero 'final'
    else {
        cola->final->siguiente = nuevo;
        cola->final = nuevo;
    }
    cola->cantidad++; // Incrementamos el contador de elementos
}

/* 
 * OPERACIÓN DESENCOLAR (Dequeue): Extrae el nodo del FRENTE de la cola.
 * Retorna el puntero al nodo extraído (quien llame a la función debe liberar la memoria o procesarlo).
 */
NodoPasajero *desencolarPasajero(ColaPasajeros *cola) {
    // Si la cola no existe o está vacía, no hay nada que desencolar
    if (cola == NULL || cola->frente == NULL) {
        return NULL;
    }

    // 1. Guardamos temporalmente el nodo del frente
    NodoPasajero *temporal = cola->frente;

    // 2. Movemos el puntero 'frente' al siguiente nodo de la cola
    cola->frente = temporal->siguiente;

    // CASO ESPECIAL: Si al desencolar la cola queda vacía, actualizamos 'final' a NULL
    if (cola->frente == NULL) {
        cola->final = NULL;
    }

    cola->cantidad--;           // Decrementamos el contador
    temporal->siguiente = NULL; // Desconectamos el nodo extraído de la estructura
    
    return temporal; // Retornamos el nodo desencolado
}

/* 
 * OPERACIÓN PEEK / CONSULTA:
 * Permite ver quién es el primer pasajero en la cola sin modificar ni desencolar nada.
 */
NodoPasajero *consultarPrimerPasajero(ColaPasajeros *cola) {
    return cola != NULL ? cola->frente : NULL;
}

/* 
 * GESTIÓN DE MEMORIA (Liberación dinamica):
 * Recorre la cola y libera uno a uno la memoria acumulada por cada nodo dinámico (free).
 * Previene fugas de memoria (Memory Leaks).
 */
void liberarColaPasajeros(ColaPasajeros *cola) {
    if (cola == NULL) {
        return;
    }

    NodoPasajero *actual = cola->frente;
    
    // Recorrido seguro usando un puntero auxiliar 'siguiente'
    while (actual != NULL) {
        NodoPasajero *siguiente = actual->siguiente; // Guardamos la referencia del siguiente antes de liberar
        free(actual);                               // Liberamos el nodo actual
        actual = siguiente;                         // Avanzamos al nodo guardado
    }

    // Reiniciamos los atributos de la estructura a su estado inicial seguro
    cola->frente = NULL;
    cola->final = NULL;
    cola->cantidad = 0;
}

/* 
 * REGISTRO DE DESTINO:
 * Reserva memoria dinámica en el Heap para un nuevo nodo de tipo 'Destino'.
 */
void registrarDestino() {
    // Asignación de memoria dinámica
    Destino *nuevoDestino = (Destino *) malloc(sizeof(Destino));
    
    // Verificación crítica: Asegurar que malloc no retornó NULL por falta de memoria RAM
    if (nuevoDestino == NULL) {
        printf("No se pudo reservar memoria.\n");
        return;
    }