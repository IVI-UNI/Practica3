
#include "calcular_primos.h"
#include "primos.h"
#include <stdio.h>

#define TAM 50847535

char *servicio[] = {"contar", "buscar"};

/** Función con parametros de entrada, nip de quien solicita el servicio, min y max indican el rango
 *Devuelve el número de primos encontrados el rango*/
int *contar_1_svc(int nip, int min, int max, struct svc_req *rqstp) {
    static int nPrimos;
    static struct timeval inicioFuncion, finFuncion;
    printf(INFO_SOLICITUD, nip, servicio[0], min, max);

    gettimeofday(&inicioFuncion, NULL);	
    nPrimos = cuenta_primos(min, max);
    gettimeofday(&finFuncion, NULL);	 

    printf(RESPUESTA_CONTAR, nPrimos, min, max);
    printf("tiempo FUNCION : %ld micro-segundos\n", ((finFuncion.tv_sec * 1000000 + finFuncion.tv_usec) - (inicioFuncion.tv_sec * 1000000 + inicioFuncion.tv_usec)));  
    return (&nPrimos);
}

/** Función con parametros de entrada, nip de quien solicita el servicio, min y max indican el rango
 *Devuelve el número de primos encontrados el rango*/
struct encontrados *buscar_1_svc(int nip, int min, int max, struct svc_req *reqstp) {
    int i;
    static struct encontrados respuesta;
    static struct timeval inicioFuncion, finFuncion;

    printf(INFO_SOLICITUD, nip, servicio[0], min, max);

    gettimeofday(&inicioFuncion, NULL);	
    respuesta.nPrimos = encuentra_primos(min, max, respuesta.vectorPrimos);
    gettimeofday(&finFuncion, NULL);	 
   printf( "Nº primos encontrados %d", respuesta.nPrimos);
    printf(RESPUESTA_ENCONTRAR);

    for (i = 0; i < respuesta.nPrimos; i++) {
        printf("%d \t ", htonl(respuesta.vectorPrimos[i]));
    }
    printf("\n");
      printf("tiempo FUNCION : %ld micro-segundos\n", ((finFuncion.tv_sec * 1000000 + finFuncion.tv_usec) - (inicioFuncion.tv_sec * 1000000 + inicioFuncion.tv_usec)));  
    return (&respuesta);
}

int cuenta_primos(int min, int max){
  int i, contador;
  contador = 0;
  
  for (i = min; i <= max; i++)
    if (esprimo(i)) contador = contador + 1;

  return (contador);
}


int encuentra_primos(int min, int max, int vector[]){
  int i, contador;

  contador = 0;

  for (i = min; i <= max; i++)
    if (esprimo(i)) vector[contador++] = (uint32_t) ntohl(i);

  return (contador);
}


int esprimo(int n){
  int i;

  for (i = 2; i*i <= n; i++)
    if ((n % i) == 0) return (0);

  return (1);
}