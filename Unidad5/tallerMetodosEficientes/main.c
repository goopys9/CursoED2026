/****************************************************************************10**
 * * Integrantes del equipo: 
 * - Saira Lucia Gutierrez Orobio
 * - Paola Andrea Arcila Solis
 * - Victor Manuel Taborda Perlaza
 * - Eileen Thaliana Montaño Garcia
 ******************************************************************************/

#include <stdio.h>   // OBLIGATORIA: Para usar printf y scanf
#include <stdlib.h>  // Para usar rand y srand
#include <time.h>    // Para usar time(NULL)

#define MAX_CONTENEDORES 30


int pesos[MAX_CONTENEDORES];
int Ncon = 0;        // Cantidad actual de contenedores registrados
int ordenado = 0;    // Bandera: 0 = desordenado, 1 = ordenado


void registrarManualmente();
void generarAleatorio();
void mostrarContenedores();
void heapify(int n, int i);
void ordenarHeapSort();
void ordenarQuickSort(int bajo, int alto);
void iniciarQuickSort();
void merge(int izq, int medio, int der);
void ordenarMergeSort(int izq, int der);
void iniciarMergeSort();
void buscarBinario();

int main() {
    int opcion;
    
    // Inicializar la semilla para los números aleatorios
    srand(time(NULL));

    do {
        
        printf("\n==================================================\n");
        printf(" SISTEMA DE ORGANIZACIÓN DE CONTENEDORES\n");
        printf(" MUELLE DE CARGA\n");
        printf("==================================================\n");
        printf("1. Registrar peso de contenedores manualmente\n");
        printf("2. Generar pesos aleatorios de contenedores\n");
        printf("3. Mostrar contenedores registrados\n");
        printf("4. Ordenar contenedores Metodo 1\n");
        printf("5. Ordenar contenedores Metodo 2\n");
        printf("6. Ordenar contenedores Metodo 3\n");
        printf("7. Buscar contenedor\n");
        printf("8. Salir\n");
        printf("==================================================\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                registrarManualmente();
                break;
            case 2:
                generarAleatorio();
                break;
            case 3:
                mostrarContenedores();
                break;
            case 4:
                ordenarHeapSort(); // Método 1: HeapSort
                break;
            case 5:
                iniciarQuickSort(); // Método 2: QuickSort
                break;
            case 6:
                iniciarMergeSort(); // Método 3: MergeSort
                break;
            case 7:
                buscarBinario();
                break;
            case 8:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 8);

    return 0;
}


void registrarManualmente() {
    printf("Ingresa el numero de contenedores (max %d): ", MAX_CONTENEDORES);
    scanf("%d", &Ncon);

    if (Ncon < 1 || Ncon > MAX_CONTENEDORES) {
        printf("Cantidad no valida.\n");
        Ncon = 0;
        return;
    }

    for (int i = 0; i < Ncon; i++) {
        printf("Ingrese el peso del contenedor %d (en kg): ", i + 1);
        scanf("%d", &pesos[i]);
    }
    
    ordenado = 0; // REQUISITO: Cambia a 0 al registrar nuevos datos
    printf("Pesos registrados exitosamente.\n");
}


void generarAleatorio() {
    printf("Ingresa el numero de contenedores a generar (max %d): ", MAX_CONTENEDORES);
    scanf("%d", &Ncon);

    if (Ncon < 1 || Ncon > MAX_CONTENEDORES) {
        printf("Cantidad no valida.\n");
        Ncon = 0;
        return;
    }

    for (int i = 0; i < Ncon; i++) {
    
        pesos[i] = 1000 + rand() % 29001;
    }

    ordenado = 0; // REQUISITO: Cambia a 0 al generar aleatorios
    printf("Pesos aleatorios generados de forma exitosa.\n");
}


void mostrarContenedores() {
    if (Ncon == 0) {
        printf("No hay contenedores registrados en el patio.\n");
        return;
    }

    printf("\nEstado actual del arreglo (Bandera Ordenado = %d):\n", ordenado);
    for (int i = 0; i < Ncon; i++) {
        printf("[%d]: %d kg\n", i, pesos[i]);
    }
    printf("\n");
}

// 4. METODO 1: HEAPSORT
void heapify(int n, int i) {
    int mayor = i; 
    int izq = 2 * i + 1; 
    int der = 2 * i + 2; 

    if (izq < n && pesos[izq] > pesos[mayor])
        mayor = izq;

    if (der < n && pesos[der] > pesos[mayor])
        mayor = der;

    if (mayor != i) {
        int temp = pesos[i];
        pesos[i] = pesos[mayor];
        pesos[mayor] = temp;

        heapify(n, mayor);
    }
}

void ordenarHeapSort() {
    if (Ncon == 0) {
        printf("No hay datos para ordenar.\n");
        return;
    }

    for (int i = Ncon / 2 - 1; i >= 0; i--)
        heapify(Ncon, i);

    for (int i = Ncon - 1; i > 0; i--) {
        int temp = pesos[0];
        pesos[0] = pesos[i];
        pesos[i] = temp;

        heapify(i, 0);
    }

    ordenado = 1; // REQUISITO: Cambia a 1 tras ordenar exitosamente
    printf("Contenedores ordenados usando Metodo 1.\n");
}

// 5. METODO 2: QUICKSORT
void ordenarQuickSort(int bajo, int alto) {
    if (bajo < alto) {
        int pivote = pesos[alto]; 
        int i = (bajo - 1);

        for (int j = bajo; j < alto; j++) {
            if (pesos[j] < pivote) {
                i++;
                int temp = pesos[i];
                pesos[i] = pesos[j];
                pesos[j] = temp;
            }
        }
        int temp = pesos[i + 1];
        pesos[i + 1] = pesos[alto];
        pesos[alto] = temp;

        int pi = i + 1;

        ordenarQuickSort(bajo, pi - 1);
        ordenarQuickSort(pi + 1, alto);
    }
}

void iniciarQuickSort() {
    if (Ncon == 0) {
        printf("No hay datos para ordenar.\n");
        return;
    }
    ordenarQuickSort(0, Ncon - 1);
    ordenado = 1; // REQUISITO: Cambia a 1 tras ordenar exitosamente
    printf("Contenedores ordenados usando Metodo 2.\n");
}

// 6. METODO 3: MERGESORT
void merge(int izq, int medio, int der) {
    int n1 = medio - izq + 1;
    int n2 = der - medio;

    int L[MAX_CONTENEDORES], R[MAX_CONTENEDORES];

    for (int i = 0; i < n1; i++) L[i] = pesos[izq + i];
    for (int j = 0; j < n2; j++) R[j] = pesos[medio + 1 + j];

    int i = 0, j = 0, k = izq;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pesos[k] = L[i];
            i++;
        } else {
            pesos[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        pesos[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        pesos[k] = R[j];
        j++;
        k++;
    }
}

void ordenarMergeSort(int izq, int der) {
    if (izq < der) {
        int medio = izq + (der - izq) / 2;
        ordenarMergeSort(izq, medio);
        ordenarMergeSort(medio + 1, der);
        merge(izq, medio, der);
    }
}

void iniciarMergeSort() {
    if (Ncon == 0) {
        printf("No hay datos para ordenar.\n");
        return;
    }
    ordenarMergeSort(0, Ncon - 1);
    ordenado = 1; // REQUISITO: Cambia a 1 tras ordenar exitosamente
    printf("Contenedores ordenados usando Metodo 3.\n");
}

// 7. BÚSQUEDA BINARIA
void buscarBinario() {
    if (Ncon == 0) {
        printf("No hay contenedores registrados.\n");
        return;
    }

    // REQUISITO: Validar la bandera antes de realizar la búsqueda
    if (ordenado == 0) {
        printf("ERROR: El arreglo NO esta ordenado. Debe ejecutar un metodo de ordenamiento antes de buscar.\n");
        return;
    }

    int pesoBuscar;
    printf("Ingrese el peso del contenedor que desea buscar: ");
    scanf("%d", &pesoBuscar);

    int inicio = 0;
    int fin = Ncon - 1;
    int encontrado = -1;

    while (inicio <= fin) {
        int mitad = inicio + (fin - inicio) / 2;

        if (pesos[mitad] == pesoBuscar) {
            encontrado = mitad;
            break; 
        }
        if (pesos[mitad] < pesoBuscar) {
            inicio = mitad + 1; 
        } else {
            fin = mitad - 1;    
        }
    }

    if (encontrado != -1) {
        printf("¡Contenedor encontrado! El peso %d kg esta en la posicion [%d].\n", pesoBuscar, encontrado);
    } else {
        printf("El contenedor con peso %d kg no se encuentra en el muelle.\n", pesoBuscar);
    }
}