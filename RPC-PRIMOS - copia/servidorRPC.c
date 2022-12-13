/**
 * servidorRPC.c
 *
 * Funciones RPC del servidor para la cuenta y búsqueda de primos.
 *
 * Radu Constantin Robu - 07/11/2022
 */

#include "primos.h"
#include <stdio.h>

#define TAM 50847535

#define MENSAJE_PETICION "El usuario %d ha pedido el servicio de %s entre %d y %d.\n"
#define MENSAJE_SERVICIO_CONTAR "Entre %d y %d hay %d números primos.\n"
#define MENSAJE_SERVICIO_BUSCAR "Entre %d y %d se han encontrado los siguientes números primos:\n"

char *SERVICIOS[] = {"contar", "buscar"};

/**
 * Función RPC para contar el número de primos entre dos valores.
 * @param min valor inferior del rango
 * @param max valor superior del rango
 * @param rqstp
 * @return número de primos contados
 */
int *contar_1_svc(int nip, int min, int max, struct svc_req *rqstp) {
    static int resultado;

    fprintf(stderr, MENSAJE_PETICION, nip, SERVICIOS[0], min, max);

    resultado = cuenta_primos(min, max);
    fprintf(stderr, MENSAJE_SERVICIO_CONTAR, min, max,
            resultado);

    return (&resultado);
}

/**
 * Función RPC para buscar los números primos entre dos valores.
 * @param min valor inferior del rango
 * @param max valor superior del rango
 * @param reqstp
 * @return número de primos contados
 */
struct primos *buscar_1_svc(int nip, int min, int max, struct svc_req *reqstp) {
    int i;
    static struct primos resultado;

    fprintf(stderr, MENSAJE_PETICION, nip, SERVICIOS[1], min, max);

    resultado.numero = encuentra_primos(min, max, resultado.vector);
    fprintf(stderr, MENSAJE_SERVICIO_BUSCAR, min, max);

    for (i = 0; i < resultado.numero; i++) {
        printf("%d ", resultado.vector[i]);
    }
    printf("\n");

    return (&resultado);
}

/**
 * Función auxiliar para contar el número de primos entre dos valores.
 * @param min valor inferior del rango
 * @param max valor superior del rango
 * @return número de primos contados
 */
int cuenta_primos(int min, int max) {
    int i, contador;

    contador = 0;

    for (i = min; i <= max; i++)
        if (esprimo(i)) contador = contador + 1;

    return (contador);
}

/**
 * Función auxiliar para encontrar los números primos entre dos valores.
 * @param min valor inferior del rango
 * @param max valor superior del rango
 * @param vector que contendrá los números primos
 * @return número de primos contados
 */
int encuentra_primos(int min, int max, int vector[]) {
    int i, contador;
    contador = 0;

    for (i = min; i <= max; i++)
        if (esprimo(i)) vector[contador++] = i;

    return (contador);
}

/**
 * Función auxiliar que devuelve si un número es primo o no.
 * @param n número de prueba
 * @return TRUE si n es primo
 */
int esprimo(int n) {
    int i;

    for (i = 2; i * i <= n; i++)
        if ((n % i) == 0) return (0);

    return (1);
}
