/*------------------------------------------------------------------------------

	"HOLA.c" : primer programa de prueba para el sistema operativo GARLIC 1.0;
	
	Imprime el t�pico mensaje "Hello world!" por una ventana de GARLIC, un
	n�mero aleatorio de veces, dentro de un rango entre 1 y 10 elevado al
	argumento ([0..3]), es decir, hasta 1, 10, 100 o 1000 iteraciones.

------------------------------------------------------------------------------*/

#include "GARLIC_API.h"			/* definici�n de las funciones API de GARLIC */

int _start(int arg)				/* funci�n de inicio : no se usa 'main' */
{
	char a[8];
	int res;
	FILE * f;

	if (arg < 0) arg = 0;			// limitar valor m�ximo y 
	else if (arg > 3) arg = 3;		// valor m�nimo del argumento
	
									// esccribir mensaje inicial
	GARLIC_printf("-- Programa OPEN -  PID (%d) --\n", GARLIC_pid());
	
	f = GARLIC_fopen("Datos/test", "rw");
	GARLIC_printf("-- Open called --\n");
	if (f == 0)
	{
		GARLIC_printf("-- Not found --\n");
		return 0;
	}
	else
		GARLIC_printf("-- File: %x --\n", f);

	res = GARLIC_fread(&a, 7, 1, f);
	a[7] = '\0';
	GARLIC_printf("-- Read called --\n");
	GARLIC_printf("-- Char readed: %s and bytes readed: %d\n", a, res);

	GARLIC_fclose(f);
	GARLIC_printf("-- Close called --\n");
	return 0;
}
