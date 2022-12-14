#include <stdio.h>

#define MINIMO 1L
#define MAXIMO 51000000L
#define PUERTO 7070
#define MAX_SERVIDORES 4
#define MAX_HILOS 4
#define INFO_SOLICITUD "Usuario con NIP:%d, SERVICIO %s y RANGO %d a %d \n"
#define RESPUESTA_CONTAR "Encontrados >> %d << entre %d y %d \n"
#define RESPUESTA_ENCONTRAR "Primos encontrados: \n"
#define PRUEBA "YO DAAAWG WTF GOING ON \n"
#define CONTAR 0
#define ENCONTRAR 1

int esprimo(int n);
int encuentra_primos(int min, int max, int vector[]);
int cuenta_primos(int min, int max);
void* funcion(void* numeroServidor);
int comprobar_hilos(int servidores);