

#include "primos.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

#define CONTAR_PRIMOS 0
#define BUSCAR_PRIMOS 1

#define MAX_ARGUMENTOS 6

#define THREAD_ID 0
#define NAMEHOST 1
#define PUERTO 2
#define NIP 1
#define SERVICIO 2
#define MIN 3
#define MAX 4

#define TAM 50847535

#define MAX_ARGUMENTOS 6
#define NUM_THREADS 4
#define NUM_PARAMETROS 4

#define ARGV_DISTRIBUCION 1
#define ARGV_NIP 2
#define ARGV_SERVICIO 3
#define ARGV_MIN 4
#define ARGV_MAX 5

#define MENSAJE_CONEXION_THREAD "Soy el thread %d y me he conectado con el servidor.\n"
#define ERROR_NUMERO_PARAMETROS "Se ha introducido un número incorrecto de parámetros.\n"
#define MENSAJE_PARAMETROS_CORRECTOS "Parámetros correctos. Empezando ejecución con threads.\n"
#define MENSAJE_RESPUESTA_CONTAR "Hay %d primos entre %d y %d.\n"
#define MENSAJE_RESPUESTA_BUSCAR "\nEntre %d y %d existen los siguientes números primos:\n"
#define MENSAJE_TIEMPO_SERVICIO "El servidor ha tardado %ld.%ld06 segundos en %s los primos entre %d y %d.\n"
#define MENSAJE_FIN_PROCESO "\nEl proceso ha acabado.\n"

const char *namehosts[] = {"10.7.1.164"};
const char *SERVICIOS[] = {"contar", "buscar"};

uint32_t parametros[NUM_THREADS][NUM_PARAMETROS];
static int num_primos_threads[NUM_THREADS];
static struct primos *primos_threads[NUM_THREADS];

void *thread_primos(void *arg);

int main(int argc, char *argv[]) {
    int num_threads[] = {0, 1, 2, 3};
    int distribucion, nip, servicio, min, max;
    int i, j, rango, primos_totales;
    pthread_t primos_tid[NUM_THREADS];

    if (argc != MAX_ARGUMENTOS) {
        printf(ERROR_NUMERO_PARAMETROS);
        exit(-3);
    }

    distribucion = atoi(argv[ARGV_DISTRIBUCION]);
    nip = atoi(argv[ARGV_NIP]);
    servicio = atoi(argv[ARGV_SERVICIO]);
    min = atoi(argv[ARGV_MIN]);
    max = atoi(argv[ARGV_MAX]);

    printf(MENSAJE_PARAMETROS_CORRECTOS);

    for (i = 0; i < distribucion; i++) {
        rango = (max - min) / distribucion;

        parametros[i][NIP] = nip;
        parametros[i][SERVICIO] = servicio;

        if (i == 0) {
            parametros[i][MIN] = min;
            parametros[i][MAX] = min + rango;
        } else {
            parametros[i][MIN] = min + (i * rango) + 1;
            parametros[i][MAX] = parametros[i][MIN] + (i * rango);
        }

        if (i > 0 && i < distribucion - 1) {
            parametros[i][MAX] = min + (i + 1) * rango;
        } else if (i == distribucion - 1) {
            parametros[i][MAX] = max;
        }

        pthread_create(&primos_tid[i], NULL, thread_primos,
                       (void *) num_threads[i]);
    }

    for (i = 0; i < distribucion; i++) {
        pthread_join(primos_tid[i], NULL);
    }

    printf(MENSAJE_FIN_PROCESO);
    exit(0);
}

/**
 * Función que se ejecuta en cada thread.
 * @param arg
 */
void *thread_primos(void *arg) {
    struct timeval tval_inicio, tval_final, tval_resultado;
    int id_thread, nip, servicio, min, max, i = 0;
    CLIENT *sv;
    int *num_primos;

    id_thread = (int) arg;
    nip = parametros[id_thread][NIP];
    servicio = parametros[id_thread][SERVICIO];
    min = parametros[id_thread][MIN];
    max = parametros[id_thread][MAX];

    sv = clnt_create(namehosts[id_thread], PRIMOS, UNO, "tcp");
    printf(MENSAJE_CONEXION_THREAD, id_thread);
    gettimeofday(&tval_inicio, NULL);

    switch (servicio) {
        case CONTAR_PRIMOS:
            num_primos = contar_1(nip, min, max, sv);
            gettimeofday(&tval_final, NULL);
            timersub(&tval_final, &tval_inicio, &tval_resultado);

            num_primos_threads[id_thread] = *num_primos;

            printf(MENSAJE_TIEMPO_SERVICIO,
                   (long int) tval_resultado.tv_sec,
                   (long int) tval_resultado.tv_usec, SERVICIOS[0], min, max);
            break;
        case BUSCAR_PRIMOS:
            primos_threads[id_thread] = buscar_1(nip, min, max, sv);
            gettimeofday(&tval_final, NULL);
            timersub(&tval_final, &tval_inicio, &tval_resultado);

            printf(MENSAJE_TIEMPO_SERVICIO,
                   (long int) tval_resultado.tv_sec,
                   (long int) tval_resultado.tv_usec, SERVICIOS[1], min, max);
            break;
    }

    clnt_destroy(sv);
    pthread_exit(0);
}

