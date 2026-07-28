# Terminal Portuario Turístico

Sistema de consola en C para la gestión de un terminal portuario turístico de Buenaventura: registro de destinos, pasajeros en fila de espera, viajes programados y embarques.

## Descripción general

El sistema permite administrar:

- **Destinos**: puertos o lugares turísticos a los que viajan los pasajeros, cada uno con su empresa transportadora.
- **Pasajeros**: se registran en una **fila de espera (cola)** asociada a un destino específico, hasta que son embarcados.
- **Viajes**: cada destino puede tener varios viajes programados, organizados en un **árbol AVL** (balanceado automáticamente) según su código, cada uno con una capacidad máxima y un contador de pasajeros embarcados.
- **Embarques**: al embarcar, se toma el primer pasajero de la fila del destino y se asocia a un viaje.


## Compilación

Para compilar en Windows con GCC:

gcc -Wall -Wextra main.c destino.c terminal.c viajes.c -o app.exe


## Menú de opciones

```
===== TERMINAL PORTUARIO TURISTICO =====
1. Registrar destino
2. Registrar pasajero
3. Mostrar pasajeros por destino
4. Registrar viaje programado
5. Buscar viaje
6. Mostrar arbol de viajes
7. Realizar embarque
8. Consultar pasajero
9. Mostrar destinos registrados
10. Mostrar estadisticas
0. Salir
======================================
```

### 1. Registrar destino
Solicita un código único, el nombre del destino y el nombre de la empresa transportadora. Si el código ya existe, se rechaza el registro.

### 2. Registrar pasajero
Pide el código del destino (debe existir previamente), el número de documento y el tipo de documento:

- `1` Cédula de Ciudadanía
- `2` Pasaporte
- `3` Tarjeta de Identidad

El pasajero se agrega al final de la fila de espera de ese destino. Un mismo documento no puede registrarse en más de una fila del sistema.

### 3. Mostrar pasajeros por destino
Solicita el código del destino y lista todos los pasajeros en su fila, indicando si están **En espera** o **Embarcado**.

### 4. Registrar viaje programado
Solicita el código del destino, un código de viaje único y la capacidad máxima. El viaje se inserta en el árbol AVL de viajes de ese destino.

### 5. Buscar viaje
Solicita el código del destino y el código del viaje; muestra su capacidad y la cantidad de pasajeros ya embarcados.

### 6. Mostrar árbol de viajes
Solicita el código del destino y lista todos sus viajes programados en orden ascendente de código (recorrido in-order del árbol AVL).

### 7. Realizar embarque
Solicita el código del destino y el código del viaje. Si hay pasajeros en espera y el viaje existe, se toma al primer pasajero de la fila (FIFO) y se marca como embarcado, incrementando el contador de embarcados del viaje.

### 8. Consultar pasajero
Solicita un número de documento y busca en todos los destinos si ese pasajero está registrado, mostrando el destino y su estado actual.

### 9. Mostrar destinos registrados
Lista todos los destinos con su código, nombre, empresa, cantidad de pasajeros en espera y si tiene viajes registrados.

### 10. Mostrar estadísticas
Muestra un resumen general:
- Total de destinos registrados
- Total de pasajeros registrados
- Total de pasajeros embarcados
- Promedio de pasajeros en espera por destino
- Promedio de pasajeros embarcados por viaje
- Destino con mayor y menor cantidad de pasajeros en espera

### 0. Salir
Libera toda la memoria reservada (colas de pasajeros y árboles de viajes) y termina el programa.

## Estructuras de datos utilizadas

- **Lista enlazada simple**: almacena los destinos registrados (`inicioDestinos`).
- **Cola (FIFO)**: gestiona el orden de llegada de los pasajeros en espera por destino.
- **Árbol AVL**: organiza los viajes programados de cada destino por código, garantizando búsquedas balanceadas en tiempo O(log n).

## Notas importantes

- Todos los datos se almacenan **en memoria**; al cerrar el programa (opción `0`), la información se pierde.
- Los códigos de destino y de viaje deben ser numéricos y únicos dentro de su ámbito (destino o viaje dentro de un destino).
- Si se ingresa una opción de menú no numérica o inválida, el sistema muestra "Opcion invalida." y vuelve a mostrar el menú.