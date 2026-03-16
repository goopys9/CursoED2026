#include <stdio.h>
#include "areas.h"
#include "volumenes.h"

int main(){

    int opcion;
    float radio, lado;

    printf("=== CALCULADORA GEOMETRICA ===\n");
    printf("1. Area de circulo\n");
    printf("2. Area de cuadrado\n");
    printf("3. Volumen de esfera\n");
    printf("4. Volumen de cubo\n");

    printf("Seleccione una opcion: ");
    //se leerá un numero entero y lo guarda en opción
    scanf("%d", &opcion);

    switch(opcion){

        case 1:
            printf("Ingrese el radio: ");
            //float y pide el radio
            scanf("%f",&radio);
            //Llama la función areaCirculo y muestra dos decimales
            printf("Area: %.2f\n", areaCirculo(radio));
        break;

        case 2:
            printf("Ingrese el lado: ");
            scanf("%f",&lado);
            printf("Area: %.2f\n", areaCuadrado(lado));
        break;

        case 3:
            printf("Ingrese el radio: ");
            scanf("%f",&radio);
            printf("Volumen: %.2f\n", volumenEsfera(radio));
        break;

        case 4:
            printf("Ingrese el lado: ");
            scanf("%f",&lado);
            printf("Volumen: %.2f\n", volumenCubo(lado));
        break;

        default:
            printf("Opcion invalida\n");
    }

    return 0;
}