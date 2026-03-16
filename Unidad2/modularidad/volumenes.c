#include "volumenes.h"
#include <math.h>

float volumenEsfera(float radio){
    //fórmula matemática para hallar el volumen
    return (4.0/3.0) * M_PI * radio * radio * radio;
}
// lado al cubo para calcular el columen
float volumenCubo(float lado){
    return lado * lado * lado;
}