#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "ejecutar.h"
#include "libmemoria.h"
#include "redirecciones.h"

/**
* Funcion que se encarga de ejecutar la orden introducida por la entrada estandar.
* param orden: orden introducida por la entrada estandar.
* paran pbackgr: entero que determina si existe bacground o no, (0,1).
* return pid: devuelve el pid del proceso.
*/
pid_t ejecutar_orden(const char *orden, int *pbackgr){

   char **args; /* Se encarga de alamacenar los comandos introducidos en la shell. */
   int entrada; 
   int salida; 
   pid_t pid; /* Se encargara del amacenar el pid del proceso. */
   
   int indice_ent = -1, indice_sal = -1; /* por defecto, no hay < ni > */
  
   if ((args = parser_orden(orden, &indice_ent, &indice_sal, pbackgr)) == NULL){ /* Controla que la orden introducida sea un ENTER. */
      
      return(-1);

   }

   pid = fork(); /* Crea una clonacion del proceso padre, lo denominamos proceso hijo. */
   
   if(pid <= -1){ /* Controlo que pueda existir un error en la creacion del nuevo proceso. */

      perror("Error en la creacion de la minishell");
      exit(-1);	/* Termina la ejecucion del proceso que lo invoca, se cierran todos los descrptores abiertos por el proceso.
      							SERVICIO POSIX.*/
	
   }else if(pid == 0){ /* Si es el proceso hijo. */
    
      if(execvp(args[0],args) == -1){ /* execvp(). Controlo el error a la hora de ejecutar la orden */
      
        perror("Error en ejecutar orden");
        exit(-1); /* Termina la ejecucion del proceso que lo invoca, se cierran todos los descriptores abiertos por del proceso.
        						 SERVICIO POSIX. */
      
   		}else{

						if(indice_ent != -1){

							redirec_entrada(args,indice_ent,&entrada); /* Llamamos a la funcion ubicada en redirecciones. */

							dup2(entrada,0); /* dup2(archivo origen, entrada teclado). Proporciona un nuevo descriptor de archivos.  SERVICIO POSIX*/
							close(entrada); /* Cierra el descriptor. SERVICIO POSIX. */
					
						}else if(indice_sal != -1){ 
							
							redirec_salida(args,indice_sal,&salida); /* Lamamos a la funcion ubucada en redirecciones. */
							
							dup2(salida,1); /* dup2(archivo origen, salida pantalla). Proporciona un nuevo descriptor de archivos. SERVICIO POSIX*/
							close(salida); /* Cierra el descriptor. SERVICIO POSIX. */

						}

	   				execvp(args[0],args); /* Cambia el codigo del programa que se esta ejecutando, por el codigo de la orden a ejecutar. 
	   																 SERVICIO POSIX. */

      }

   }else{ /* Si es el proceso padre. */

				free_argumentos(args); /* Llamo a la funcion que se encuentra en libmemoria.c */

	 }
	 
	 return pid;
	
}
 
/**
* Funcion encargada de controlar si existe background en la orden y finalizar el proceso hijo.
* param orden: recibe la orden introducida por la entrada estandar.
*/
void ejecutar_linea_ordenes(const char *orden) {

   pid_t pid; /* Almacena el pid del proceso */

   int backgr;/* Indica si la orden tiene background, o no.(1, 0), lo determina la funcion ejecutar_orden() */

   pid = ejecutar_orden(orden, &backgr);
   
   if(backgr == 0){ /* Si la orden contiene '&'. */ 
        
        /*
        
        El proceso minishell padre simplemente no debe esperar la finalización
        del proceso minishell hijo y podrá volver a ejecutar inmediatamente otra iteración
        del bucle de la función main(). Esto significa que el proceso minishell padre y el
        proceso minishell hijo podrán ejecutar concurrentemente órdenes. 
        
        */
    

   }else if(backgr == 1){ /* La orden no tiene '&' */
        
        waitpid(pid,NULL,0); /* pid_t waitpid(pid_t pid, int *status, int options); Esperamos a que termine la minishell hija.
        												SERVICIO POSIX. */
        
   }

}  

/* 
* Funcion que se encarga de separar las ordenes cuando haya ";" y llamar a ejecutar linea de ordenes para cada una de ellas.
* param orden: orden introducida por la entrada estandar.
*/
void ejecutar_ordenes_separadas(const char *orden){

	char *orden_completa = strdup(orden); /* Devuelve un puntero a una nueva cadena, que es la duplicada de la cadena orden,
																	 				 lo utilizamos para no pederla informacion de la orden. */
	
	char *orden_separada = strsep(&orden_completa, ";"); /* Separa la orden cada vez que encuentre un ';' */
	
	while(orden_separada != NULL){ /* Mientras exista una orden para ejecutar.  */
	
		ejecutar_linea_ordenes(orden_separada);
		
		orden_separada = strsep(&orden_completa , ";");
	
	}

} 
