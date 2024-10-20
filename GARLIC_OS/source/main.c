/*------------------------------------------------------------------------------

	"main.c" : fase 1 / programador M

	Programa de prueba de carga de un fichero ejecutable en formato ELF,
	pero sin multiplexaci�n de procesos ni utilizar llamadas a _gg_escribir().
------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include "garlic_system.h"	// definici�n de funciones y variables de sistema
#include "garlic_mem.h"

extern int * punixTime;		// puntero a zona de memoria con el tiempo real

/* Inicializaciones generales del sistema Garlic */
//------------------------------------------------------------------------------
void inicializarSistema() {
//------------------------------------------------------------------------------
	
	consoleDemoInit();		// inicializar console, solo para esta simulaci�n
	
	_gd_seed = *punixTime;	// inicializar semilla para n�meros aleatorios con
	_gd_seed <<= 16;		// el valor de tiempo real UNIX, desplazado 16 bits

	if (!_gm_initFS())
	{
		printf("ERROR: �no se puede inicializar el sistema de ficheros!");
		exit(0);
	}
}

static void	test_names()
{
	intFunc start;
	int		correct;

	correct = 0;
	start = _gm_cargarPrograma("TEST");
	if (!start)
		correct++;
	else
		printf("ERROR TEST1: este fichero no deberia existir ni deberia ser encontrado.\n");

	start = _gm_cargarPrograma("HOLA");
	if (start)
		correct++;
	else
		printf("ERROR TEST2: no ha encontrado el programa.\n");

	start = _gm_cargarPrograma("HOOLA");
	if (start)
		printf("ERROR TEST3: ha cargado el programa aunque no sigue convenio.\n");
	else
		correct++;
	
	start = _gm_cargarPrograma("hola");
	if (start)
		printf("ERROR TEST4: ha cargado el programa aunque no sigue convenio.\n");
	else
		correct++;

	printf("***** Tests correctos %d/4.\n", correct);
}

static void	test_progams()
{
	intFunc start;

	printf("*** Carga de programa HOLA.elf\n");
	start = _gm_cargarPrograma("HOLA");
	if (start)
	{
		printf("*** Direccion de arranque :\n\t\t%p\n", start);
		printf("*** Pulse tecla \'START\' ::\n\n");
		do
		{	swiWaitForVBlank();
			scanKeys();
		} while ((keysDown() & KEY_START) == 0);
		
		start(1);
	}
	else
		printf("*** Programa \"HOLA\" NO cargado\n");

	printf("\n\n\n*** Carga de programa PRNT.elf\n");
	start = _gm_cargarPrograma("PRNT");
	if (start)
	{
		printf("*** Direccion de arranque :\n\t\t%p\n", start);
		printf("*** Pulse tecla \'START\' ::\n\n");
		do
		{	swiWaitForVBlank();
			scanKeys();
		} while ((keysDown() & KEY_START) == 0);
		
		start(1);
	}
	else
		printf("*** Programa \"PRNT\" NO cargado\n");
}

static void test_header()
{
	intFunc start = _gm_cargarPrograma("OPEN");
	if (start == (intFunc) 0)
		printf("Error\n");
	printf("*** Direccion de arranque :\n\t\t%p\n", start);
	start(1);
}


//------------------------------------------------------------------------------
int main(int argc, char **argv) {
//------------------------------------------------------------------------------
	inicializarSistema();

	printf("********************************");
	printf("*                              *");
	printf("* Sistema Operativo GARLIC 1.0 *");
	printf("*                              *");
	printf("********************************");
	printf("*** Inicio fase 1_M\n");

	//test_names();
	//TODO : wait for a button to be pressed and then load new programs
	//test_progams();

	test_header();

	printf("*** Final fase 1_M\n");

	while (1)
	{
		swiWaitForVBlank();
	}							// parar el procesador en un bucle infinito
	return 0;
}
