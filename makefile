CC = gcc
CFLAGS = -g -Wall

carrera: simula_car.o
	$(CC) $(CFLAGS) simula_car.o -o carrera -lpthread

simula_car.o: simula_car.c simula_car.h
	$(CC) $(CFLAGS) -c simula_car.c

.PHNOY: clean
clean: 
	rm -f *.o carrera
