#include "terminal.h"

/* 
 * FUNCIÓN AUXILIAR: maximo
 * Retorna el mayor de dos números enteros.
 * Se utiliza para calcular la nueva altura de un nodo basándose en la altura
 * máxima de sus dos subárboles (izquierdo y derecho).
 */
static int maximo(int a, int b) {
    return a > b ? a : b;
}

/* 
 * FUNCIÓN AUXILIAR: alturaNodo
 * Devuelve la altura almacenada en el nodo.
 * Maneja de forma segura los punteros NULL (un nodo nulo tiene altura 0),
 * evitando errores de acceso a memoria (Segmentation Fault).
 */
static int alturaNodo(NodoViaje *nodo) {
    if (nodo == NULL) {
        return 0;
    }
    return nodo->altura;
}

/* 
 * CÁLCULO DEL FACTOR DE EQUILIBRIO (FE)
 * FE = Altura(Subárbol Izquierdo) - Altura(Subárbol Derecho)
 * 
 * Criterio de balance en un Árbol AVL:
 *  - FE = 0, 1 o -1  --> El nodo está balanceado.
 *  - FE > 1          --> El nodo está desbalanceado hacia la IZQUIERDA.
 *  - FE < -1         --> El nodo está desbalanceado hacia la DERECHA.
 */
static int factorEquilibrio(NodoViaje *nodo) {
    if (nodo == NULL) {
        return 0;
    }
    return alturaNodo(nodo->izq) - alturaNodo(nodo->der);
}

/* 
 * ROTACIÓN SIMPLE A LA DERECHA (Caso Izquierda-Izquierda / LL)
 * Se usa cuando el subárbol izquierdo está más pesado.
 * 
 * Proceso:
 * 1. El hijo izquierdo (x) pasa a ser la nueva raíz del subárbol.
 * 2. El subárbol derecho de x (t2) se reasigna al lado izquierdo de la antigua raíz (y).
 * 3. La antigua raíz (y) pasa a ser el hijo derecho de la nueva raíz (x).
 * 4. Se recalculan las alturas de abajo hacia arriba (primero 'y', luego 'x').
 */
static NodoViaje *rotarDerecha(NodoViaje *y) {
    NodoViaje *x = y->izq;
    NodoViaje *t2 = x->der;

    // Reorganización de punteros
    x->der = y;
    y->izq = t2;

    // Actualización de alturas tras la rotación
    y->altura = maximo(alturaNodo(y->izq), alturaNodo(y->der)) + 1;
    x->altura = maximo(alturaNodo(x->izq), alturaNodo(x->der)) + 1;

    // Retorna la nueva raíz del subárbol
    return x;
}

/* 
 * ROTACIÓN SIMPLE A LA IZQUIERDA (Caso Derecha-Derecha / RR)
 * Se usa cuando el subárbol derecho está más pesado.
 * 
 * Proceso:
 * 1. El hijo derecho (y) pasa a ser la nueva raíz del subárbol.
 * 2. El subárbol izquierdo de y (t2) se reasigna al lado derecho de la antigua raíz (x).
 * 3. La antigua raíz (x) pasa a ser el hijo izquierdo de la nueva raíz (y).
 * 4. Se recalculan las alturas de abajo hacia arriba (primero 'x', luego 'y').
 */
static NodoViaje *rotarIzquierda(NodoViaje *x) {
    NodoViaje *y = x->der;
    NodoViaje *t2 = y->izq;

    // Reorganización de punteros
    y->izq = x;
    x->der = t2;

    // Actualización de alturas tras la rotación
    x->altura = maximo(alturaNodo(x->izq), alturaNodo(x->der)) + 1;
    y->altura = maximo(alturaNodo(y->izq), alturaNodo(y->der)) + 1;

    // Retorna la nueva raíz del subárbol
    return y;
}

/* 
 * FUNCIÓN PRINCIPAL DE BALANCEO
 * Actualiza la altura del nodo actual y verifica si rompe la propiedad AVL.
 * Si detecta un desbalance (|FE| > 1), aplica las rotaciones correspondientes.
 */
static NodoViaje *balancearArbol(NodoViaje *nodo) {
    if (nodo == NULL) {
        return NULL;
    }

    // 1. Actualizar la altura del nodo actual basándose en sus hijos
    nodo->altura = maximo(alturaNodo(nodo->izq), alturaNodo(nodo->der)) + 1;

    // 2. Obtener el Factor de Equilibrio
    int fe = factorEquilibrio(nodo);

    // CASO 1: Desbalance hacia la Izquierda (FE > 1)
    if (fe > 1) {
        // Subcaso Izquierda-Derecha (LR):
        // Si el hijo izquierdo tiene un FE negativo, requiere primero una rotación a la izquierda.
        if (factorEquilibrio(nodo->izq) < 0) {
            nodo->izq = rotarIzquierda(nodo->izq);
        }
        // Aplica la rotación a la derecha (Cubre casos LL y LR resuelto)
        return rotarDerecha(nodo);
    }

    // CASO 2: Desbalance hacia la Derecha (FE < -1)
    if (fe < -1) {
        // Subcaso Derecha-Izquierda (RL):
        // Si el hijo derecho tiene un FE positivo, requiere primero una rotación a la derecha.
        if (factorEquilibrio(nodo->der) > 0) {
            nodo->der = rotarDerecha(nodo->der);
        }
        // Aplica la rotación a la izquierda (Cubre casos RR y RL resuelto)
        return rotarIzquierda(nodo);
    }

    // Si no hay desbalance, el nodo no requiere cambios y se retorna tal cual
    return nodo;
}

// Inserta un nuevo viaje en el árbol AVL
NodoViaje *insertarViajeEnArbol(NodoViaje *raiz, int codigo, int capacidad) {

    // Si el árbol está vacío, crea el primer nodo
    if (raiz == NULL) {
        NodoViaje *nuevo = (NodoViaje *) malloc(sizeof(NodoViaje));

        // Verifica si la memoria se reservó correctamente
        if (nuevo == NULL) {
            printf("No se pudo reservar memoria.\n");
            return NULL;
        }

        // Inicializa los datos del nuevo viaje
        nuevo->codigo = codigo;
        nuevo->capacidad = capacidad;
        nuevo->altura = 1;
        nuevo->pasajerosEmbarcados = 0;
        nuevo->izq = NULL;
        nuevo->der = NULL;

        return nuevo;
    }

    // Si el código es menor, lo inserta en el subárbol izquierdo
    if (codigo < raiz->codigo) {
        raiz->izq = insertarViajeEnArbol(raiz->izq, codigo, capacidad);

    // Si el código es mayor, lo inserta en el subárbol derecho
    } else if (codigo > raiz->codigo) {
        raiz->der = insertarViajeEnArbol(raiz->der, codigo, capacidad);

    // Si el código ya existe, no permite duplicados
    } else {
        printf("El codigo del viaje ya existe.\n");
        return raiz;
    }

    // Balancea el árbol para mantener el AVL organizado
    return balancearArbol(raiz);
}

// Busca un viaje por su código dentro del árbol
NodoViaje *buscarViajeEnArbol(NodoViaje *raiz, int codigo) {

    // Si no existe el nodo, retorna NULL
    if (raiz == NULL) {
        return NULL;
    }

    // Si encuentra el código, devuelve el nodo
    if (codigo == raiz->codigo) {
        return raiz;
    }

    // Busca en el subárbol izquierdo
    if (codigo < raiz->codigo) {
        return buscarViajeEnArbol(raiz->izq, codigo);
    }

    // Busca en el subárbol derecho
    return buscarViajeEnArbol(raiz->der, codigo);
}

// Muestra todos los viajes en orden ascendente por código
void mostrarViajesEnOrden(NodoViaje *raiz) {

    // Si el árbol está vacío, termina
    if (raiz == NULL) {
        return;
    }

    // Recorre primero la izquierda
    mostrarViajesEnOrden(raiz->izq);

    // Muestra la información del viaje actual
    printf("Codigo: %d | Capacidad: %d | Embarcados: %d\n",
           raiz->codigo, raiz->capacidad, raiz->pasajerosEmbarcados);

    // Recorre el subárbol derecho
    mostrarViajesEnOrden(raiz->der);
}

// Libera toda la memoria ocupada por el árbol de viajes
void liberarArbolViajes(NodoViaje *raiz) {

    // Si no hay nodos, termina
    if (raiz == NULL) {
        return;
    }

    // Libera primero los hijos
    liberarArbolViajes(raiz->izq);
    liberarArbolViajes(raiz->der);

    // Libera el nodo actual
    free(raiz);
}

// Función para registrar un nuevo viaje
void registrarViaje() {

    // Variables para guardar los datos ingresados por el usuario
    int codigoDestino;
    int codigoViaje;
    int capacidad;

    // Solicita el código del destino
    printf("\nIngrese codigo del destino: ");
    scanf("%d", &codigoDestino);

    // Limpia el buffer del teclado
    limpiarEntrada();
}