#include <stdio.h> //peermite usar el printf y scanf
#include <stdbool.h> //permite usar bool (true/false)

#define N 100 //tamaño de arreglos

// Prototipos de funciones
void mostrar(int arr[], int n);
void bubbleSort(int arr[], int n); //funciones necesarias 
void insertionSort(int arr[], int n);
void selectionSort(int arr[], int n);

int main() {
    int arreglo[N]; //vector donde se guardan los números
    int n = 0; //números que el usuario decidió ingresar
    int opcion; //opción que el usuario elige en el menú
    bool tieneDatos = false;//Un interruptor. Empieza en 'apagado' (false) porque no hay datos.

    do {
        printf("\n--- SISTEMA DE ORDENAMIENTO (PARCIAL 1) ---\n");
        printf("1. Registrar datos en el vector\n");
        printf("2. Mostrar el arreglo original\n");
        printf("3. Ordenar mediante metodo Burbuja\n");
        printf("4. Ordenar mediante metodo Seleccion\n");
        printf("5. Ordenar mediante metodo Insercion\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion); //lee la elección del usuario

        // Restricción: Solo la opción 1 habilitada al inicio
        if (!tieneDatos && opcion >= 2 && opcion <= 5) { //Condición que desabilita las demás opciones sin antes llenar el vector..
            printf("\n[!] ERROR: Primero debe registrar datos (Opcion 1).\n");
            continue; //Salta directo al inicio del 'do' sin pasar por el switch.
        }

        switch (opcion) { //Evalúa qué número eligió el usuario.
            case 1:
                printf("¿Cuantos elementos desea ingresar? ");
                scanf("%d", &n); //recibe un número entero y guarda(&)en n 
                for (int i = 0; i < n; i++) { //Este ciclo se repite tantas veces como el valor de n
                    printf("Dato [%d]: ", i + 1); //Muestra un mensaje indicando qué número de dato estás ingresando.
                    scanf("%d", &arreglo[i]); //guarda cada número directamente en la posición i del arreglo.
                }
                tieneDatos = true; 
                printf("-> Datos guardados correctamente.\n");
                break;

            case 2:
                printf("\nArreglo original: ");
                mostrar(arreglo, n);
                break;

                //Se utiliza una copia para ver el arreglo original
            case 3: {
                int copia[N]; // el ordenamiento se hace sobre una copia sin dañar el arreglo original
                for(int i=0; i<n; i++) copia[i] = arreglo[i]; 
                bubbleSort(copia, n);
                break;
            }
// i = 0 se empieza desde la primera posición
//i < n: El ciclo se repite hasta llegar al último número que el usuario escribió.
            case 4: {
                int copia[N];
                for(int i=0; i<n; i++) copia[i] = arreglo[i]; 
                selectionSort(copia, n); //le da la copia para ordenar 
                break;
            }

            case 5: {
                int copia[N];
                for(int i=0; i<n; i++) copia[i] = arreglo[i];
                insertionSort(copia, n);
                break;
            }

            case 6:
                printf("Cerrando programa...\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 6); //se repite mientras sea diferente de 6

    return 0;
}

void mostrar(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]); //imprime los números
    }
    printf("\n");
}

void bubbleSort(int arr[], int n) {
    printf("\n--- Iniciando Burbuja ---\n");
    for (int i = 0; i < n - 1; i++) { //controla la pasada
        for (int j = 0; j < n - i - 1; j++) { //bucle interno: compara
            if (arr[j] > arr[j + 1]) { // Si el de la izquierda es mayor que el de la derecha hay que intercambiarlos
                int temp = arr[j]; // Guardas el valor de la izquierda temporal (el "vaso vacío"). para no perderlo
                arr[j] = arr[j + 1]; // Mueves el pequeño a la izquierda.
                arr[j + 1] = temp; // Mueves el grande a la derecha.
                printf("[INTERCAMBIO] %d <-> %d: ", arr[j], arr[j+1]); 
                mostrar(arr, n);
            }
        }
        printf(">> CICLO (Pasada %d) FINALIZADO\n", i + 1); // Cumple punto 1
    }
    printf("\nResultado final: ");
    mostrar(arr, n);
}

void selectionSort(int arr[], int n) {
    printf("\n--- Iniciando Seleccion ---\n");
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i; //hipótesis del número más pequeño
        for (int j = i + 1; j < n; j++) { //Empiezas a buscar desde la siguiente posición hasta el final
            if (arr[j] < arr[minIndex]) { //Comparas cada número con el que creías que era el más pequeño.
                minIndex = j;
            }
        }
        int temp = arr[i]; //vaso temporal
        arr[i] = arr[minIndex]; // El más pequeño viaja a su lugar definitivo
        arr[minIndex] = temp; // El que estaba antes se va a donde estaba el pequeño
        
        printf("[INTERCAMBIO] %d colocado en posicion %d: ", arr[i], i); //imprime elmovimiento fisico que se hizo
        mostrar(arr, n);
        printf(">> CICLO %d FINALIZADO\n", i + 1); // Cumple punto 1 
    }
}

void insertionSort(int arr[], int n) {
    printf("\n--- Iniciando Insercion ---\n");
    // El ciclo empieza en 1 porque el primer elemento (0) ya se considera "ordenado"
    for (int i = 1; i < n; i++) {
        int clave = arr[i]; //'clave' es el numero que tenemos en la mano para acomodar
        int j = i - 1; // 'j' es la posicion del numero a la izquierda de la clave

        /* Mientras el numero a la izquierda (arr[j]) sea mayor que nuestra clave, 
           lo movemos una posicion a la derecha para abrir espacio */
        while (j >= 0 && arr[j] > clave) {
            arr[j + 1] = arr[j]; // Desplazamos el numero mayor a la derecha

            //Mostrar el movimiento interno
            printf("[MOVIMIENTO] Desplazando %d: ", arr[j]);
            mostrar(arr, n);
            j--; // Seguimos comparando hacia la izquierda
        }

        // Cuando encontramos el lugar correcto, soltamos la clave ahi
        arr[j + 1] = clave;
        printf("[INSERCION] Clave %d insertada: ", clave);
        mostrar(arr, n);
        printf(">> CICLO %d FINALIZADO\n", i); // Cumple punto 1 
    }
}