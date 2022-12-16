
#include "primos.h"
#include  "calcular_primos.h"
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
#include <pthread.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define CONTAR_PRIMOS 0
#define BUSCAR_PRIMOS 1

#define MAX_ARGUMENTOS 6

#define THREAD_ID 0
#define NAMEHOST 1
#define NIP 1
#define SERVICIO 2


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

const char *namehosts[] = {"127.0.0.1"};
const char *servicio[] = {"contar", "buscar"};
const int static hilosID[] = {0,1,2,3};
int static hilosCreados[4];
int static minHilo[4];
int static maxHilo[4];
int static posiciones[4];
int static bytes_leidos[4];
int static bytes_leidos_total[4];
int static servicioSolicitado;
int static nip;
int static sock[4];
struct hostent *hostIP[4];
struct sockaddr_in sockDirecciones[4];
int static *ptrPrimosHilo[4];
int static primosHilo[4];
int static primosHiloSuma[4];
int static nPrimosLeer[4];
int static *intPtr[4];
char static *respuesta[4];
int static parametrosHilos[4][4];
int static primosTotal;
int static desplazar[4] = {0,0,0,0};
uint32_t static matrizPrimos[4][MAXIMO];
uint32_t contarPrimos = 0;
uint32_t obtenerPrimos = 1;

struct timeval inicioServicio;
bool primerHilo = true;

pthread_mutex_t static mutexCond;
pthread_cond_t static otroHilo;

const char* vectorServidores[] = {"127.0.0.1", "127.0.0.1","127.0.0.1","127.0.0.1"};


uint32_t parametros[NUM_THREADS][NUM_PARAMETROS];
static int num_primos_threads[NUM_THREADS];
static struct encontrados *primos_threads[NUM_THREADS];

void *thread_primos(void *arg);

int main(int argc, char *argv[]) {
    //int distribucion, nip, servicio, min, max;
    //int i, j, rango, primos_totales;
    pthread_t hilo[MAX_HILOS];
	int i=0;
	int servidores = atoi(argv[1]);
	nip = atoi(argv[2]);
	servicioSolicitado = atoi(argv[3]);
	int minTotal = atoi(argv[4]);
	int maxTotal = atoi(argv[5]);
	int rangoServidor;
	int limitePeticion = 10000000;
	int restoPeticion = maxTotal%limitePeticion;
	int minPeticion = minTotal;
	int numerosIntervalo=(maxTotal-minTotal+1);
	int maxPeticion;
	//argumentos_struct argumentos_hilo;
	int finiquito = 0;
	int hilosEsperar = 0;
	int hiloLibre=0;
   
   	/**Comienza el contador para el programa entero**/
	struct timeval inicioPrograma, finPrograma, finServicio;
	gettimeofday(&inicioPrograma, NULL);												

    /*
    if (argc != MAX_ARGUMENTOS) {
        printf(ERROR_NUMERO_PARAMETROS);
        exit(-3);
    }
    distribucion = atoi(argv[ARGV_DISTRIBUCION]);
    nip = atoi(argv[ARGV_NIP]);
    servicio = atoi(argv[ARGV_SERVICIO]);
    min = atoi(argv[ARGV_MIN]);
    max = atoi(argv[ARGV_MAX]);*/

    printf(INFO_SOLICITUD, nip, servicio[servicioSolicitado], minTotal, maxTotal);

    /**Inicializar mutex y variable condición**/
	if (pthread_mutex_init(&mutexCond, NULL) != 0) {                                    
    	perror("mutex_lock");                                                       
    	exit(1);                                                                    
  	}  

    pthread_cond_init(&otroHilo, NULL);


/**Trocear el intervalo entre servidores**/
	if (servidores>1){
		rangoServidor = numerosIntervalo/servidores;
	}else{
		rangoServidor = numerosIntervalo;
	}

	if (limitePeticion > rangoServidor){
		maxPeticion = minTotal+rangoServidor-1;
	}else{
		maxPeticion = minTotal+limitePeticion-1;
	}
	
	/**Bucle para la creación de hilos**/
	while(finiquito != 1){	
		
		if (maxPeticion == maxTotal){
			finiquito = 1;
		}
		/**Semáforo de control de creación de hilos**/
		pthread_mutex_lock(&mutexCond);
		if (i == servidores){
			pthread_cond_wait(&otroHilo, &mutexCond);
			hiloLibre = comprobar_hilos(servidores);
			if (servidores == 1){
				i = 0;
				hiloLibre = i;
			}
		}
		pthread_mutex_unlock(&mutexCond);

		/**Asignación de intervalo de numeros al hilo**/
		minHilo[hiloLibre]= minPeticion;
		maxHilo[hiloLibre] = maxPeticion;


		/**Creación del nuevo hilo**/
		if((pthread_create(&hilo[hiloLibre], NULL, &funcion, (void *)&hilosID[hiloLibre])) != 0)
			perror("Error al crear el hilo");
		
		/**Actualizacion del array de control de la creación de hilos**/
		pthread_mutex_lock(&mutexCond);
		hilosCreados[hiloLibre] =1;
		pthread_mutex_unlock(&mutexCond);
		
		/**Cálculo del nuevo rango del próximo hilo**/
		minPeticion = maxPeticion + 1;
		if((maxPeticion + restoPeticion)==maxTotal){
			maxPeticion = maxTotal;
		}else{
			maxPeticion = maxPeticion + (rangoServidor);
			if (maxPeticion > maxTotal)
				maxPeticion = maxTotal;
			if ((maxPeticion-minPeticion)>limitePeticion)
				maxPeticion=minPeticion+limitePeticion-1;
		}

		if (i != servidores){
			i++;
			hiloLibre = i;
		}		

	}

	/**Esperar a que los hilos terminen**/
	if (servidores == 1){
		if((pthread_join(hilo[0], NULL)) != 0)
			perror("Error al esperar al hilo");
		close(sock[0]);
	}else{
		for(int s=0; s < i; s++){
			//printf("esperando el hilo: %d \n", s);
			if((pthread_join(hilo[s], NULL)) != 0)
				perror("Error al esperar al hilo");
			close(sock[s]);
		}
	}

	gettimeofday(&finServicio, NULL);					//Fin del contador de servicio

	/**Mostrar por pantalla los resultados de las peticiones**/
	if(servicioSolicitado == contarPrimos){
		primosTotal = primosHiloSuma[0]+ primosHiloSuma[1]+ primosHiloSuma[2]+primosHiloSuma[3];
		printf("PRIMOS entre %d <--> %d ===> %d \n", minTotal, maxTotal, primosTotal);
		pthread_cond_destroy(&otroHilo);
	}else if(servicioSolicitado == obtenerPrimos){
		printf("Primos obtenidos \n");
		for(int s=0; s < servidores; s++)
			for(int k=0; k< primos_threads[s]->nPrimos; k++)
				printf( "%d\t", htonl(primos_threads[s]->vectorPrimos[k]));
		pthread_cond_destroy(&otroHilo);
	}
	printf("\n");

	gettimeofday(&finPrograma, NULL);					//Fin del contador de programa
  printf("tiempo PROGRAMA : %ld micro-segundos\n", ((finPrograma.tv_sec * 1000000 + finPrograma.tv_usec) - (inicioPrograma.tv_sec * 1000000 + inicioPrograma.tv_usec)));
  printf("tiempo SERVICIO: %ld micro-segundos\n", ((finServicio.tv_sec * 1000000 + finServicio.tv_usec) - (inicioServicio.tv_sec * 1000000 + inicioServicio.tv_usec)));

  exit(0);
}

void* funcion(void* numeroServidor){

	/**Obtención del nº de servidor pasado como argumento**/
	int servidor = *((int *) numeroServidor);
    CLIENT *sv;
 	

    sv = clnt_create(vectorServidores[servidor], PRIMOS, UNO, "tcp");

	printf("HILO--> %d\tmin--> %d\tmax-->%d\t\n", servidor, minHilo[servidor], maxHilo[servidor]);

	if(primerHilo){
		primerHilo = false;
		gettimeofday(&inicioServicio, NULL);							//iniciar el contador de servicio
	}

	/**Leer la respuesta del servidor a la peticion de servicio**/
    switch (servicioSolicitado) {
        case CONTAR:
	printf("ENTRO EN EL SWITCH\n");
            ptrPrimosHilo[servidor] = contar_1(nip, minHilo[servidor], maxHilo[servidor], sv);
		printf("EJECUTADA LA FUNCION\n");
		printf( "Nº de primos en HILO>%d<  --> %d \n", servidor, *(ptrPrimosHilo[servidor]));
		primosHilo[servidor] =(int) *(ptrPrimosHilo[servidor]);
			primosHiloSuma[servidor] = primosHiloSuma[servidor] + primosHilo[servidor];

			/**Actualizar semáforo de control de creacion de hilos**/
			pthread_mutex_lock(&mutexCond);
			hilosCreados[servidor] =0;
			pthread_cond_signal(&otroHilo);
			pthread_mutex_unlock(&mutexCond);
            break;
        case ENCONTRAR:
            primos_threads[servidor] = buscar_1(nip, minHilo[servidor], maxHilo[servidor], sv);
		
		/**Actualizar semáforo de control de creacion de hilos**/
		pthread_mutex_lock(&mutexCond);
		hilosCreados[servidor] =0;
		pthread_cond_signal(&otroHilo);
		pthread_mutex_unlock(&mutexCond);

            break;
    }

    clnt_destroy(sv);
    pthread_exit(0);

}


/**Función que devuelve el nº de hilo que no está ocupado**/
int comprobar_hilos(int servidores){
	for (int hilo = 0; hilo < servidores; hilo++){
		if (hilosCreados[hilo]== 0)
			return hilo;
	}
}