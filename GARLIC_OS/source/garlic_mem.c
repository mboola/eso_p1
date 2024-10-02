/*------------------------------------------------------------------------------

	"garlic_mem.c" : fase 1 / programador M

	Funciones de carga de un fichero ejecutable en formato ELF, para GARLIC 1.0

------------------------------------------------------------------------------*/
#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "garlic_system.h"	// definici�n de funciones y variables de sistema

#define INI_MEM 0x01002000		// direcci�n inicial de memoria para programas



/* _gm_initFS: inicializa el sistema de ficheros, devolviendo un valor booleano
					para indiciar si dicha inicializaci�n ha tenido �xito;
*/
int _gm_initFS()
{
	return 0;
}


/* _gm_cargarPrograma: busca un fichero de nombre "(keyName).elf" dentro del
					directorio "/Programas/" del sistema de ficheros y carga
					los segmentos de programa a partir de una posici�n de
					memoria libre, efectuando la reubicaci�n de las referencias
					a los s�mbolos del programa seg�n el desplazamiento del
					c�digo en la memoria destino;
	Par�metros:
		keyName ->	string de 4 caracteres con el nombre en clave del programa
	Resultado:
		!= 0	->	direcci�n de inicio del programa (intFunc)
		== 0	->	no se ha podido cargar el programa
*/
intFunc _gm_cargarPrograma(char *keyName)
{

	return ((intFunc) 0);
}
