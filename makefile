CC = gcc
CFLAGS = -g -Wall

minishell: ejecutar.o entrada_minishell.o libmemoria.o minishell.o redirecciones.o libshell.a
	$(CC) $(CFLAGS) ejecutar.o entrada_minishell.o 	libmemoria.o minishell.o redirecciones.o -L./ -lshell -o minishell

ejecutar.o: ejecutar.c ejecutar.h parser.h libmemoria.h redirecciones.h
	$(CC) $(CFLAGS) -c ejecutar.c

libmemoria.o: libmemoria.c libmemoria.h
	$(CC) $(CFLAGS) -c libmemoria.c

entrada_minishell.o: entrada_minishell.c entrada_minishell.h
	$(CC) $(CFLAGS) -c entrada_minishell.c

minishell.o: minishell.c internas.h ejecutar.h entrada_minishell.h
	$(CC) $(CFLAGS) -c minishell.c

redirecciones.o: redirecciones.c redirecciones.h
	$(CC) $(CFLAGS) -c redirecciones.c

libshell.a: internas.o parser.o
	ar -r libshell.a parser.o internas.o

.PHONY: clean #Evita que no se ejecute el  make porque ya existe un fichero llamado clean
clean: #Limpia los archivos .o 
	rm -f minishell *.o
