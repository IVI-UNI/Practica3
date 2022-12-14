#include <stdio.h>

#define MINIMO 1L
#define MAXIMO 51000000L
#define PUERTO 7070
#define MAX_SERVIDORES 4
#define SERVICIOS {"CONTAR", "ENCONTRAR"}
#define INFO_SOLICITUD "Usuario con NIP:%d, SERVICIO %s y RANGO %d a %d \n"
#define RESPUESTA_CONTAR "Encontrados >> %d << entre %d y %d \n"
#define RESPUESTA_ENCONTRAR "Primos encontrados: \n"