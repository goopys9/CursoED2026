// SISTEMA DE GESTIÓN DE TALLER "POWERFIX CENTER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//DEFINICIÓN DE CONSTANTES Y ESTRUCTURAS
#define MAX_PLACA    20
#define MAX_SERVICIO 100
#define MAX_REPUESTO 100

/* ----- Nodo de la Cola de Vehículos ----- */
typedef struct NodoVehiculo {
    char placa[MAX_PLACA];
    char servicio[MAX_SERVICIO];
    struct NodoVehiculo *siguiente;
} NodoVehiculo;

/* ----- Cola (FIFO) de Vehículos ----- */
typedef struct {
    NodoVehiculo *frente;
    NodoVehiculo *fondo;
    int cantidad;
} Cola;

/* ----- Nodo de la Pila de Repuestos ----- */
typedef struct NodoRepuesto {
    char nombre[MAX_REPUESTO];
    int cantidad;
    struct NodoRepuesto *siguiente;
} NodoRepuesto;

/* ----- Pila (LIFO) de Repuestos ----- */
typedef struct {
    NodoRepuesto *tope;
    int cantidad;
} Pila;

//PROTOTIPOS DE FUNCIONES
/* Cola */
void          inicializarCola(Cola *cola);
int           colaVacia(Cola *cola);
void          encolar(Cola *cola, const char *placa, const char *servicio);
NodoVehiculo *desencolar(Cola *cola);
void          mostrarCola(Cola *cola);
void          liberarCola(Cola *cola);

/* Pila */
void           inicializarPila(Pila *pila);
int            pilaVacia(Pila *pila);
void           apilar(Pila *pila, const char *nombre, int cantidad);
NodoRepuesto  *desapilar(Pila *pila);
void           mostrarPila(Pila *pila);
void           liberarPila(Pila *pila);

/* Lógica del taller */
void registrarVehiculo(Cola *cola);
void registrarRepuesto(Pila *pila);
void atenderVehiculo(Cola *cola, Pila *pila);
void consultarCola(Cola *cola);
void consultarPila(Pila *pila);

/* Utilidades */
void limpiarBuffer(void);
void pausar(void);
void limpiarPantalla(void);
void mostrarBanner(void);
void mostrarMenuPrincipal(void);

// IMPLEMENTACIÓN: COLA (FIFO)
void inicializarCola(Cola *cola) {
    cola->frente   = NULL;
    cola->fondo    = NULL;
    cola->cantidad = 0;
}

int colaVacia(Cola *cola) {
    return (cola->frente == NULL);
}

/*
 * encolar – Agrega un vehículo al fondo de la cola.
 * El primer vehículo en llegar es el primero en ser atendido (FIFO).
 */
void encolar(Cola *cola, const char *placa, const char *servicio) {
    NodoVehiculo *nuevo = (NodoVehiculo *)malloc(sizeof(NodoVehiculo));
    if (nuevo == NULL) {
        printf("\n  [ERROR] Memoria insuficiente al encolar vehiculo.\n");
        return;
    }
    strncpy(nuevo->placa,    placa,    MAX_PLACA    - 1);
    strncpy(nuevo->servicio, servicio, MAX_SERVICIO - 1);
    nuevo->placa[MAX_PLACA - 1]       = '\0';
    nuevo->servicio[MAX_SERVICIO - 1] = '\0';
    nuevo->siguiente = NULL;

    if (colaVacia(cola)) {
        cola->frente = nuevo;
        cola->fondo  = nuevo;
    } else {
        cola->fondo->siguiente = nuevo;
        cola->fondo = nuevo;
    }
    cola->cantidad++;
}

/*
 * desencolar – Retira el vehículo del frente de la cola.
 * El llamador es responsable de liberar la memoria del nodo retornado.
 */
NodoVehiculo *desencolar(Cola *cola) {
    if (colaVacia(cola)) {
        return NULL;
    }
    NodoVehiculo *extraido = cola->frente;
    cola->frente = cola->frente->siguiente;
    if (cola->frente == NULL) {
        cola->fondo = NULL;
    }
    extraido->siguiente = NULL;
    cola->cantidad--;
    return extraido;
}

void mostrarCola(Cola *cola) {
    if (colaVacia(cola)) {
        printf("\n  La cola de vehiculos esta vacia.\n");
        return;
    }
    NodoVehiculo *actual = cola->frente;
    int posicion = 1;
    printf("\n  %-5s %-15s %-40s\n", "Pos.", "Placa", "Servicio Solicitado");
    printf("  %-5s %-15s %-40s\n",
           "-----", "---------------", "----------------------------------------");
    while (actual != NULL) {
        printf("  %-5d %-15s %-40s\n", posicion, actual->placa, actual->servicio);
        actual = actual->siguiente;
        posicion++;
    }
    printf("\n  Total de vehiculos en espera: %d\n", cola->cantidad);
}

void liberarCola(Cola *cola) {
    NodoVehiculo *actual = cola->frente;
    while (actual != NULL) {
        NodoVehiculo *tmp = actual->siguiente;
        free(actual);
        actual = tmp;
    }
    inicializarCola(cola);
}

// IMPLEMENTACIÓN: PILA (LIFO)
void inicializarPila(Pila *pila) {
    pila->tope     = NULL;
    pila->cantidad = 0;
}

int pilaVacia(Pila *pila) {
    return (pila->tope == NULL);
}

/*
 * apilar – Agrega un repuesto en el tope de la pila.
 * El ultimo repuesto en ingresar es el primero en salir (LIFO).
 */
void apilar(Pila *pila, const char *nombre, int cantidad) {
    NodoRepuesto *nuevo = (NodoRepuesto *)malloc(sizeof(NodoRepuesto));
    if (nuevo == NULL) {
        printf("\n  [ERROR] Memoria insuficiente al apilar repuesto.\n");
        return;
    }
    strncpy(nuevo->nombre, nombre, MAX_REPUESTO - 1);
    nuevo->nombre[MAX_REPUESTO - 1] = '\0';
    nuevo->cantidad  = cantidad;
    nuevo->siguiente = pila->tope;
    pila->tope       = nuevo;
    pila->cantidad++;
}

/*
 * desapilar – Retira el repuesto del tope de la pila.
 * El llamador es responsable de liberar la memoria del nodo retornado.
 */
NodoRepuesto *desapilar(Pila *pila) {
    if (pilaVacia(pila)) {
        return NULL;
    }
    NodoRepuesto *extraido = pila->tope;
    pila->tope = pila->tope->siguiente;
    extraido->siguiente = NULL;
    pila->cantidad--;
    return extraido;
}

void mostrarPila(Pila *pila) {
    if (pilaVacia(pila)) {
        printf("\n  La pila de repuestos esta vacia.\n");
        return;
    }
    NodoRepuesto *actual = pila->tope;
    int posicion = 1;
    printf("\n  %-5s %-35s %-10s\n", "Pos.", "Repuesto", "Cantidad");
    printf("  %-5s %-35s %-10s\n",
           "-----", "-----------------------------------", "----------");
    while (actual != NULL) {
        if (posicion == 1) {
            printf("  %-5d %-35s %-10d  <-- TOPE\n",
                   posicion, actual->nombre, actual->cantidad);
        } else {
            printf("  %-5d %-35s %-10d\n",
                   posicion, actual->nombre, actual->cantidad);
        }
        actual = actual->siguiente;
        posicion++;
    }
    printf("\n  Total de repuestos en inventario: %d tipo(s)\n", pila->cantidad);
}

void liberarPila(Pila *pila) {
    NodoRepuesto *actual = pila->tope;
    while (actual != NULL) {
        NodoRepuesto *tmp = actual->siguiente;
        free(actual);
        actual = tmp;
    }
    inicializarPila(pila);
}

/* =========================================================
 *   LÓGICA PRINCIPAL DEL TALLER
 * ========================================================= */

/*
 * registrarVehiculo – Solicita placa y servicio, luego encola el vehiculo.
 */
void registrarVehiculo(Cola *cola) {
    char placa[MAX_PLACA];
    char servicio[MAX_SERVICIO];

    printf("\n  === REGISTRO DE VEHICULO ===\n");
    printf("  Ingrese la placa del vehiculo : ");
    fgets(placa, sizeof(placa), stdin);
    placa[strcspn(placa, "\n")] = '\0';

    if (strlen(placa) == 0) {
        printf("  [ERROR] La placa no puede estar vacia.\n");
        return;
    }

    printf("  Ingrese el servicio solicitado: ");
    fgets(servicio, sizeof(servicio), stdin);
    servicio[strcspn(servicio, "\n")] = '\0';

    if (strlen(servicio) == 0) {
        printf("  [ERROR] El servicio no puede estar vacio.\n");
        return;
    }

    encolar(cola, placa, servicio);
    printf("\n  [OK] Vehiculo '%s' registrado correctamente en la cola.\n", placa);
    printf("       Posicion actual en cola: %d\n", cola->cantidad);
}

/*
 * registrarRepuesto – Solicita nombre y cantidad, luego apila el repuesto.
 */
void registrarRepuesto(Pila *pila) {
    char nombre[MAX_REPUESTO];
    int  cantidad;

    printf("\n  === REGISTRO DE REPUESTO ===\n");
    printf("  Ingrese el nombre del repuesto : ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    if (strlen(nombre) == 0) {
        printf("  [ERROR] El nombre del repuesto no puede estar vacio.\n");
        return;
    }

    printf("  Ingrese la cantidad disponible : ");
    if (scanf("%d", &cantidad) != 1 || cantidad <= 0) {
        printf("  [ERROR] Cantidad invalida. Debe ser un numero mayor a cero.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    apilar(pila, nombre, cantidad);
    printf("\n  [OK] Repuesto '%s' (cantidad: %d) apilado correctamente.\n",
           nombre, cantidad);
}

/*
 * atenderVehiculo – Atiende al primer vehiculo de la cola.
 *
 * Proceso:
 *   1. Muestra el primer vehiculo de la cola (sin desencolar aun).
 *   2. Solicita nombre y cantidad del repuesto requerido.
 *   3. Busca el repuesto en la pila con una pila auxiliar que preserva
 *      el principio LIFO. Los nodos fisicos se MUEVEN reasignando
 *      punteros (sin malloc/free intermedios): cero fragmentacion,
 *      cero riesgo de fuga de memoria parcial.
 *   4. Si existe y hay stock suficiente: descuenta cantidad,
 *      elimina el repuesto si llega a 0, y desencola el vehiculo.
 *   5. Si no existe o stock insuficiente: muestra mensaje,
 *      restaura la pila moviendo los nodos de vuelta, no desencola.
 *   6. Los nodos de la auxiliar regresan a la pila principal
 *      tambien por reasignacion de punteros.
 */
void atenderVehiculo(Cola *cola, Pila *pila) {
    if (colaVacia(cola)) {
        printf("\n  [AVISO] No hay vehiculos en espera. La cola esta vacia.\n");
        return;
    }

    /* Paso 1: Mostrar el primer vehiculo */
    printf("\n  === ATENCION DE VEHICULO ===\n");
    printf("  Vehiculo en el frente de la cola:\n");
    printf("    Placa   : %s\n", cola->frente->placa);
    printf("    Servicio: %s\n", cola->frente->servicio);

    /* Paso 2: Solicitar repuesto */
    char nombreRepuesto[MAX_REPUESTO];
    int  cantidadRequerida;

    printf("\n  Ingrese el repuesto requerido  : ");
    fgets(nombreRepuesto, sizeof(nombreRepuesto), stdin);
    nombreRepuesto[strcspn(nombreRepuesto, "\n")] = '\0';

    if (strlen(nombreRepuesto) == 0) {
        printf("  [ERROR] El nombre del repuesto no puede estar vacio.\n");
        return;
    }

    printf("  Ingrese la cantidad a utilizar : ");
    if (scanf("%d", &cantidadRequerida) != 1 || cantidadRequerida <= 0) {
        printf("  [ERROR] Cantidad invalida.\n");
        limpiarBuffer();
        return;
    }
    limpiarBuffer();

    /* Paso 3: Buscar repuesto con pila auxiliar (preserva LIFO) */
    printf("\n  Buscando repuesto '%s' en el inventario...\n", nombreRepuesto);

    Pila auxiliar;
    inicializarPila(&auxiliar);

    NodoRepuesto *encontrado    = NULL;
    int           elementosRet  = 0;

    /*
     * Desapilamos hacia la auxiliar hasta encontrar el repuesto.
     * Los nodos fisicos se mueven reasignando punteros: sin malloc
     * ni free intermedios, cero riesgo de fuga de memoria parcial.
     * La auxiliar queda con los elementos en orden inverso.
     */
    while (!pilaVacia(pila)) {
        NodoRepuesto *actual = desapilar(pila);

        /* Comparacion insensible a mayusculas */
        if (strcasecmp(actual->nombre, nombreRepuesto) == 0) {
            encontrado = actual;
            break;
        }
        /* Mover el nodo real a la auxiliar reasignando su puntero */
        actual->siguiente = auxiliar.tope;
        auxiliar.tope     = actual;
        auxiliar.cantidad++;
        elementosRet++;
    }

    /* Paso 4: Evaluar resultado */
    if (encontrado == NULL) {
        /* No existe en la pila */
        printf("\n  [AVISO] El repuesto '%s' NO existe en el inventario.\n",
               nombreRepuesto);
        printf("          No se puede completar la atencion del vehiculo.\n");

    } else if (encontrado->cantidad < cantidadRequerida) {
        /* Existe pero stock insuficiente */
        printf("\n  [AVISO] Stock insuficiente para '%s'.\n", nombreRepuesto);
        printf("          Disponible : %d unidad(es)\n", encontrado->cantidad);
        printf("          Requerido  : %d unidad(es)\n", cantidadRequerida);
        printf("          No se puede completar la atencion del vehiculo.\n");

        /* Devolver el nodo encontrado intacto a la pila principal
         * moviendo el puntero, sin malloc nuevo */
        encontrado->siguiente = pila->tope;
        pila->tope            = encontrado;
        pila->cantidad++;
        encontrado = NULL;

    } else {
        /* Stock suficiente: descontar cantidad */
        encontrado->cantidad -= cantidadRequerida;

        if (encontrado->cantidad == 0) {
            printf("\n  [OK] Se utilizaron %d unidad(es) de '%s'.\n",
                   cantidadRequerida, nombreRepuesto);
            printf("       Stock agotado: el repuesto se retira del inventario.\n");
            free(encontrado);
        } else {
            printf("\n  [OK] Se utilizaron %d unidad(es) de '%s'.\n",
                   cantidadRequerida, nombreRepuesto);
            printf("       Stock restante: %d unidad(es).\n", encontrado->cantidad);
            /* Devolver el nodo actualizado a la pila principal
             * moviendo el puntero, sin malloc nuevo */
            encontrado->siguiente = pila->tope;
            pila->tope            = encontrado;
            pila->cantidad++;
            encontrado = NULL;
        }

        /* Desencolar el vehiculo atendido */
        NodoVehiculo *atendido = desencolar(cola);
        printf("  Vehiculo '%s' atendido y retirado de la cola.\n", atendido->placa);
        free(atendido);
    }

    /*
     * Paso 5: Restaurar nodos de la auxiliar en la pila principal.
     * Los nodos fisicos viajan de vuelta reasignando punteros:
     * sin malloc ni free, orden LIFO perfectamente restaurado.
     */
    if (elementosRet > 0) {
        printf("\n  Restaurando %d elemento(s) retirado(s) temporalmente...\n",
               elementosRet);
    }

    while (!pilaVacia(&auxiliar)) {
        NodoRepuesto *nodo = desapilar(&auxiliar);
        /* Reinsertar el nodo real en el tope de la pila principal */
        nodo->siguiente = pila->tope;
        pila->tope      = nodo;
        pila->cantidad++;
    }

    if (elementosRet > 0) {
        printf("  Pila restaurada correctamente (principio LIFO preservado).\n");
    }
}

void consultarCola(Cola *cola) {
    printf("\n  === COLA DE VEHICULOS EN ESPERA ===");
    mostrarCola(cola);
}

void consultarPila(Pila *pila) {
    printf("\n  === INVENTARIO DE REPUESTOS (PILA) ===");
    mostrarPila(pila);
}

/* =========================================================
 *   UTILIDADES
 * ========================================================= */

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar(void) {
    printf("\n  Presione ENTER para continuar...");
    limpiarBuffer();
}

void limpiarPantalla(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarBanner(void) {
    printf("  +==================================================+\n");
    printf("  |      TALLER AUTOMOTRIZ  POWERFIX CENTER          |\n");
    printf("  |      Sistema de Gestion de Vehiculos             |\n");
    printf("  |      Estructuras de Datos - U. del Pacifico      |\n");
    printf("  +==================================================+\n");
}

void mostrarMenuPrincipal(void) {
    printf("\n  +--------------------------------------------------+\n");
    printf("  |                 MENU PRINCIPAL                   |\n");
    printf("  +--------------------------------------------------+\n");
    printf("  |  [1] Registrar vehiculo en cola                  |\n");
    printf("  |  [2] Registrar repuesto en inventario            |\n");
    printf("  |  [3] Atender vehiculo (cola + inventario)        |\n");
    printf("  |  [4] Consultar cola de vehiculos                 |\n");
    printf("  |  [5] Consultar inventario de repuestos           |\n");
    printf("  |  [6] Salir del sistema                           |\n");
    printf("  +--------------------------------------------------+\n");
    printf("  Seleccione una opcion: ");
}

/* =========================================================
 *   FUNCION PRINCIPAL
 * ========================================================= */

int main(void) {
    Cola cola;
    Pila pila;
    int  opcion;

    inicializarCola(&cola);
    inicializarPila(&pila);

    limpiarPantalla();
    mostrarBanner();

    do {
        mostrarMenuPrincipal();

        if (scanf("%d", &opcion) != 1) {
            limpiarBuffer();
            opcion = 0;
        } else {
            limpiarBuffer();
        }

        limpiarPantalla();
        mostrarBanner();

        switch (opcion) {
            case 1:
                registrarVehiculo(&cola);
                break;
            case 2:
                registrarRepuesto(&pila);
                break;
            case 3:
                atenderVehiculo(&cola, &pila);
                break;
            case 4:
                consultarCola(&cola);
                break;
            case 5:
                consultarPila(&pila);
                break;
            case 6:
                printf("\n  Liberando toda la memoria utilizada...\n");
                liberarCola(&cola);
                liberarPila(&pila);
                printf("  Memoria liberada correctamente.\n");
                printf("\n  Gracias por usar POWERFIX CENTER. Hasta pronto!\n\n");
                break;
            default:
                printf("\n  [ERROR] Opcion invalida. Ingrese un numero del 1 al 6.\n");
                break;
        }

        if (opcion != 6) {
            pausar();
            limpiarPantalla();
            mostrarBanner();
        }

    } while (opcion != 6);

    return 0;
}