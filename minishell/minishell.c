#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#include "internas.h"
#include "entrada_minishell.h"
#include "ejecutar.h"

/**
* Funcion que se encarga de tratar la señal.
*/
static void manejar_sigchild(int signo){

	int estado;
	
	waitpid(-1,&estado,WNOHANG); /* Funcion que se encarga de realizar la espera de los procesos hijos, con WNOHNG,
																	estamos diciendo que si no hay hijos por los que esperar, waitpid debe retornar inmediatamente. 
																	SERVICIO POSIX. */

}

/**
* Funcion principal del programa, se encarga de llamar a las diferentes funciones para determinar que tipo de orden se va a ejecutar.
*/
int main(int argc, char *argv[]) {

   char buf[BUFSIZ]; /* Bufer de entrada de datos */

   struct sigaction sa;/* Estructura que sirve para manejar la señal correspondiente. 
													Puede ignorar la señal, o llamar a la rutina de tratamiento de la señal.*/
   
   sa.sa_handler = manejar_sigchild; /* Especifico el procedimiento para tratarla señal, en este caso sera la funcion 
																				"manejar_sigchild" definida anteriormente. */

   sa.sa_flags = SA_NOCLDSTOP | SA_RESTART; /* sa_flags, se encarga de modificar el comportamiento de la señal:
																							SA_NOCLDSTOP: modifica el conportamiento si la señal es del procrso hijo.
																							SA_RESTART: modifica el comportamiento si ocurre algo inesperado. */

	sigaction(SIGCHLD, &sa, NULL); /* (numero_señal, funcion_gestion_señal, almacena_funcion_modificada);  
																		Permite configurar el manejo de señales, es decir, especificar un manejador para la señal sa. 
																		SIGCHLD es una señal al proceso padre cuando el proceso hijo termina.*/


   while(1){
	
		imprimir_prompt(); /* Llamada a funcion que se encarga de imprimir el prompt de la minishell */

		leer_linea_ordenes(buf); /* Llamada a funcion que se encarga leer la linea de ordenes de la entrada estandar y la almacena
															como una cadena de caracteres en buf */

		if((strchr(buf,';')) != NULL){ /* Buscamos la primera aparicion del caracter ';', si dicho caracter existe en la orden introducida,
																		lo trataremos mediante la funcion ejecutar_ordenes_separadas. */
	  
	  	ejecutar_ordenes_separadas(buf);

		}else if(strcmp(buf,"exit") == 0){ /* Si la orden introducida es 'exit', detengo la ejecucion del programa. */
	
	  	exit(0); /* Termina la ejecucion del proceso que lo invoca, se cierran todos los descriptores de archivos abiertos por el proceso,
	  						SERVICIO POSIX. */
		
		}else{ /*Si la orden introducida no es exit, determino si dicha orden es interna(1) o externa(0). */
	     
	  	if(es_ord_interna(buf) == 1){ /* cd, pwd, declare, umask, son ordenes internas(ya implementadas)*/

	    	ejecutar_ord_interna(buf); /* Llamo a la funcion que ejecuta la orden interna. */
	    
	  	}else if(es_ord_interna(buf) == 0){ /* Ordenes en las que el codigo no esta ya implementado. */

	    	ejecutar_linea_ordenes(buf); /* Llamo a la funcion que ejecuta la orden externa. */

	 	}
	 
		}
	
 	}

   	return(0);

}
