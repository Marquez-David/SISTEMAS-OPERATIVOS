#ifndef EJECUTAR_H
#define EJECUTAR_H

pid_t ejecutar_orden(const char *orden, int *pbackgr);
void ejecutar_linea_ordenes(const char *orden);
void ejecutar_ordenes_separadas(const char *orden);

#endif
