#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define N_COCHES 8

//Tipo de datos que representa un coche
typedef struct {

	int id;
	char *cadena;
	
} coche_t; 

pthread_mutex_t llave; 

coche_t Coches[N_COCHES]; //Array de coches, almacena loscoches creados.

volatile int clasificacion_ids[N_COCHES]; //Almacenara los ids de los coches ganadores segun el tiempo de llegada, pos 0 el ganador.
volatile int j = 0; /* Contador que se encargara de recorrer el array de la clasificacion. */


//Funcion ejecutada por los hilos
void *funcion_coche(coche_t *pcoche){

	int aleatorio;
	unsigned int semilla = (pcoche->id) + getpid(); // semilla generacion num. aleatorios.
	
	printf("Salida de %s %d\n", pcoche->cadena, pcoche->id);
	fflush (stdout);
	
	//generar numeros aleatorios con funcion re-entrante rand_r()
	aleatorio = rand_r(&semilla) %10;
	
	sleep(aleatorio);
	
	printf("Llegada de %s %d\n", pcoche->cadena, pcoche->id);

	pthread_mutex_lock(&llave); /* Bloqueo del hilo. */
	
	clasificacion_ids[j] = pcoche->id;
	j++;
	
	pthread_mutex_unlock(&llave);/* Desbloqueo el hilo. */

	pthread_exit(NULL); /* Finalizo la ejecucion del hilo. */

}

int main(void){

	pthread_t hilosCoches[N_COCHES]; // tabla con los identificadores de los hilos
	int i;
	
	pthread_mutex_init(&llave,NULL);
	
	printf("Se incia proceso de creacion de hilos... \n\n");
	printf("SALIDA DE COCHES\n");
	
	for(i = 0; i<N_COCHES;i++){
	
		/* Inicializar la estructura de cada coche.*/
		Coches[i].id = i;
		Coches[i].cadena = "coche";
		
		/* Creo un hilo para cada coche */
		pthread_create(&hilosCoches[i],NULL,(void *)&funcion_coche,&Coches[i]);
	
	}
	
	printf("Proceso de creacion de hilos terminado\n\n");
	
	for(i=0;i<N_COCHES;i++){
	
		/* Hago que el hilo padre espere por los hilos creados.*/ 
		pthread_join(hilosCoches[i],NULL);
	
	}
	
	printf("Todos los coches han LLEGADO A LA META \n");
	
	printf("CLASIFICACION FINAL: --------\n");
	
	for(i=0;i<N_COCHES;i++){
	
		printf("Nº: %d %s %d",i+1,Coches[i].cadena,clasificacion_ids[i]);
		printf("\n");
	
	}
	
	printf("------------------------------------\n");
		
	return 0;
	
}
