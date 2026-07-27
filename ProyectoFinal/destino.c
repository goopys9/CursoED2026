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
    
    // Solicita y lee el código numérico del nuevo destino
    printf("\nIngrese codigo del destino: ");
    scanf("%d", &nuevoDestino->codigo);
    limpiarEntrada();

    // Verifica duplicado: si ya existe un destino con ese código, aborta
    if (buscarDestinoPorCodigo(nuevoDestino->codigo) != NULL) {
        printf("El codigo ya existe.\n");
        free(nuevoDestino); // libera memoria asignada antes de salir
        return;
    }

    // Lee el nombre del destino (cadena) y remueve el '\n' final
    printf("Ingrese nombre del destino: ");
    fgets(nuevoDestino->nombre, MAX_NOMBRE, stdin);
    nuevoDestino->nombre[strcspn(nuevoDestino->nombre, "\n")] = '\0';

    // Lee el nombre de la empresa responsable del destino
    printf("Ingrese nombre de la empresa: ");
    fgets(nuevoDestino->empresa, MAX_NOMBRE, stdin);
    nuevoDestino->empresa[strcspn(nuevoDestino->empresa, "\n")] = '\0';

    // Inicializa la cola de pasajeros y punteros de árbol/lista asociados
    nuevoDestino->cola.frente = NULL;
    nuevoDestino->cola.final = NULL;
    nuevoDestino->cola.cantidad = 0;
    nuevoDestino->raizViajes = NULL; // raíz del árbol de viajes (si aplica)
    nuevoDestino->siguiente = NULL;  // enlace para la lista de destinos

    // Inserta el destino en la estructura global y confirma al usuario
    agregarDestino(nuevoDestino);
    printf("Destino registrado correctamente.\n");
}

void registrarPasajero() {
    int codigoDestino;
    int documento;
    int tipoDocumento;

    // Solicita el código del destino donde se registrará el pasajero
    printf("\nIngrese codigo del destino: ");
    scanf("%d", &codigoDestino);
    limpiarEntrada();

    // Busca el destino por código; si no existe, informa y sale
    Destino *destino = buscarDestinoPorCodigo(codigoDestino);
    if (destino == NULL) {
        printf("Destino no encontrado.\n");
        return;
    }

    // Solicita y lee el número de documento del pasajero
    printf("Ingrese numero de documento: ");
    scanf("%d", &documento);
    limpiarEntrada();

    // Muestra opciones para el tipo de documento y lee la elección
    printf("Seleccione tipo de documento:\n");
    printf("1. Cedula de Ciudadania\n");
    printf("2. Pasaporte\n");
    printf("3. Tarjeta de Identidad\n");
    scanf("%d", &tipoDocumento);
    limpiarEntrada();

    // Valida que el tipo de documento esté dentro de las constantes permitidas
    if (!documentoValido(tipoDocumento)) {
        printf("Tipo de documento no permitido.\n");
        return;
    }

    // Verifica que el pasajero no esté ya registrado en otra cola
    if (existePasajeroEnSistema(documento)) {
        printf("El pasajero ya existe en otra cola.\n");
        return;
    }

    // Reserva memoria para el nuevo nodo de pasajero
    NodoPasajero *nuevoPasajero = (NodoPasajero *) malloc(sizeof(NodoPasajero));
    if (nuevoPasajero == NULL) {
        printf("No se pudo reservar memoria.\n");
        return;
    }

    // Inicializa los campos del nuevo pasajero antes de encolarlo
    nuevoPasajero->documento = documento;
    nuevoPasajero->tipoDocumento = (TipoDocumento) tipoDocumento;
    nuevoPasajero->estado = ESTADO_ESPERA;
    nuevoPasajero->siguiente = NULL;

    // Encola el pasajero en la cola del destino y actualiza el contador global
    encolarPasajero(&destino->cola, nuevoPasajero);
    totalPasajerosRegistrados++;
    printf("Pasajero registrado en la fila del destino.\n");
}

void mostrarPasajerosPorDestino() {
    int codigoDestino;
    printf("\nIngrese codigo del destino: ");
    scanf("%d", &codigoDestino);
    limpiarEntrada();

    // ============================================================================
// MOSTRAR PASAJEROS EN COLA DE ESPERA
// Explicación para sustentación:
// Recorre la cola (FIFO) de un destino específico sin modificarla para 
// listar a los pasajeros que están esperando su viaje.
// ============================================================================
    Destino *destino = buscarDestinoPorCodigo(codigoDestino);
    if (destino == NULL) {
        printf("Destino no encontrado.\n");
        return;
    }

    printf("\nPasajeros del destino %s:\n", destino->nombre);
    // Validación previa: evita intentar recorrer una cola sin elementos
    if (verificarColaVacia(&destino->cola)) {
        printf("No hay pasajeros en espera.\n");
        return;
    }

    // Recorregordo lineal de la lista enlazada interna de la cola (desde el frente hasta el final)
    NodoPasajero *actual = destino->cola.frente;
    while (actual != NULL) {
        // Uso del operador ternario para mostrar una etiqueta legible según el enum/constante de estado
        printf("Documento: %d | Estado: %s\n", actual->documento,
               actual->estado == ESTADO_ESPERA ? "En espera" : "Embarcado");
        actual = actual->siguiente;
    }
}

// ============================================================================
// REALIZAR EMBARQUE DE UN PASAJERO
// Explicación para sustentación:
// Mueve al primer pasajero de la cola (FIFO) hacia su viaje asignado en el árbol de viajes.
// Aplica desencolado, actualización de estadísticas globales/locales y liberación de memoria.
// ============================================================================
void realizarEmbarque() {
    int codigoDestino;
    int codigoViaje;

    printf("\nIngrese codigo del destino: ");
    scanf("%d", &codigoDestino);
    limpiarEntrada(); // Limpia el buffer para evitar saltos de línea pendientes en el teclado

    // 1. Búsqueda y validación del destino
    Destino *destino = buscarDestinoPorCodigo(codigoDestino);
    if (destino == NULL) {
        printf("Destino no encontrado.\n");
        return;
    }

    // 2. Verificar que haya al menos un pasajero en la cola de este destino
    if (verificarColaVacia(&destino->cola)) {
        printf("No hay pasajeros en espera.\n");
        return;
    }

    printf("Ingrese codigo del viaje: ");
    scanf("%d", &codigoViaje);
    limpiarEntrada();

    // 3. Búsqueda del viaje dentro de la estructura jerárquica (Árbol Binario de Búsqueda)
    NodoViaje *viaje = buscarViajeEnArbol(destino->raizViajes, codigoViaje);
    if (viaje == NULL) {
        printf("No existe un viaje programado con ese codigo.\n");
        return;
    }

    // 4. Operación FIFO: Sacamos al primer pasajero que llegó a la cola
    NodoPasajero *pasajero = desencolarPasajero(&destino->cola);
    if (pasajero == NULL) {
        printf("No fue posible embarcar.\n");
        return;
    }

    // 5. Actualización de estado y contadores de control
    pasajero->estado = ESTADO_EMBARCADO;
    viaje->pasajerosEmbarcados++;        // Contador por viaje específico
    totalPasajerosEmbarcados++;          // Contador global del sistema

    // 6. Liberación de memoria dinámica (malloc previo al encolar) para evitar memory leaks
    free(pasajero);
    printf("Pasajero embarcado correctamente.\n");
}

// ============================================================================
// CONSULTAR PASAJERO POR DOCUMENTO
// Explicación para sustentación:
// Realiza una búsqueda anidada: recorre la lista de destinos y, por cada destino,
// busca el número de documento en su respectiva cola de pasajeros.
// Complejidad: O(D * P) donde D = número de destinos y P = pasajeros por cola.
// ============================================================================
void consultarPasajero() {
    int documento;
    printf("\nIngrese numero de documento: ");
    scanf("%d", &documento);
    limpiarEntrada();

    // Recorrido de la lista principal de destinos (nivel exterior)
    Destino *actual = inicioDestinos;
    while (actual != NULL) {
        
        // Recorrido de la cola de pasajeros del destino actual (nivel interior)
        NodoPasajero *aux = actual->cola.frente;
        while (aux != NULL) {
            if (aux->documento == documento) {
                printf("Pasajero encontrado en el destino %s.\n", actual->nombre);
                printf("Estado: %s\n", aux->estado == ESTADO_ESPERA ? "En espera" : "Embarcado");
                return; // Corta la función en cuanto encuentra la primera coincidencia
            }
            aux = aux->siguiente;
        }
        actual = actual->siguiente; // Pasa al siguiente destino
    }

    printf("No se encontro el pasajero.\n");
}