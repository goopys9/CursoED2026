void mostrarDestino(Destino *destino) {
    if (destino == NULL) {
        return;
    }

    printf("Codigo: %d\n", destino->codigo);
    printf("Nombre: %s\n", destino->nombre);
    printf("Empresa: %s\n", destino->empresa);
    printf("Pasajeros en espera: %d\n", destino->cola.cantidad);
    printf("Viajes registrados: ");
    if (destino->raizViajes == NULL) {
        printf("No hay viajes\n");
    } else {
        printf("Si\n");
    }
    printf("-------------------------------\n");
}

void mostrarTodosDestinos() {
    Destino *actual = inicioDestinos;
    if (actual == NULL) {
        printf("No hay destinos registrados.\n");
        return;
    }

    printf("\nDestinos registrados:\n");
    while (actual != NULL) {
        mostrarDestino(actual);
        actual = actual->siguiente;
    }
}

void liberarSistema() {
    Destino *actual = inicioDestinos;
    while (actual != NULL) {
        Destino *siguiente = actual->siguiente;
        liberarColaPasajeros(&actual->cola);
        liberarArbolViajes(actual->raizViajes);
        free(actual);
        actual = siguiente;
    }

    inicioDestinos = NULL;
    totalPasajerosRegistrados = 0;
    totalPasajerosEmbarcados = 0;
    totalDestinos = 0;
}

void mostrarEstadisticas() {
    Destino *actual = inicioDestinos;
    int totalEspera = 0;
    int totalViajes = 0;
    int totalEmbarcadosPorViaje = 0;
    Destino *destinoMayor = NULL;
    Destino *destinoMenor = NULL;
    int mayorEspera = -1;
    int menorEspera = -1;
}