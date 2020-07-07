#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>

/* funcion que abre el archivo situado en la posicion indice_entrada+1 */
/* de la orden args y elimina de ella la redireccion completa          */
                                  
void redirec_entrada(char **args, int indice_entrada, int *entrada){
   
	if((*entrada = open(args[indice_entrada+1],O_RDONLY)) == -1){ /* Si hay un error en abrir el archivo */
	
			perror("Error en la redireccion de entrada.");
			exit(-1); /* Termina la ejecucion del proceso que lo invoca, se cierran toos los descriptres abiertos.
									 SERVICIO POSIX. */
	
	}else{

			*entrada = open( args[indice_entrada+1],O_RDONLY); /* open(ruta_del_archivo,modo_lectura), se encarga de abrir el archivo
																														en modo lectura, especificado en args. */
	
	}
	

}

/* funcion que abre el archivo situado en la posicion indice_salida+1 */
/* de la orden args y elimina de ella la redireccion completa         */

void redirec_salida(char **args, int indice_salida, int *salida){ /*Si hay un error en abrir el archivo */

	if((*salida = open(args[indice_salida+1],O_WRONLY)) == -1){
	
			perror("Error en la redireccion de la salida.");
			exit(-1); /* Termina la ejecucion del proceso que lo invoca, se cierran todos los descriptres abiertos.
									 SERVICIO POSIX. */
	
	}else{

			*salida = open( args[indice_salida+1], O_WRONLY); /* open(ruta_del_archivo,modo_escrituta) */

	}

}

