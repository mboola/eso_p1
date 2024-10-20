/*------------------------------------------------------------------------------

	"HOLA.c" : primer programa de prueba para el sistema operativo GARLIC 1.0;
	
	Imprime el t�pico mensaje "Hello world!" por una ventana de GARLIC, un
	n�mero aleatorio de veces, dentro de un rango entre 1 y 10 elevado al
	argumento ([0..3]), es decir, hasta 1, 10, 100 o 1000 iteraciones.

------------------------------------------------------------------------------*/

#include <GARLIC_API.h>			/* definici�n de las funciones API de GARLIC */

int _start(int arg)				/* funci�n de inicio : no se usa 'main' */
{	
	if (arg < 0) arg = 0;			// limitar valor m�ximo y 
	else if (arg > 3) arg = 3;		// valor m�nimo del argumento
	
									// esccribir mensaje inicial
	GARLIC_printf("-- Programa OPEN 2 -  PID (%d) --\n", GARLIC_pid());
	GARLIC_printf("-- Hehe 3 --\n");
	GARLIC_fopen("test", 0);
	GARLIC_printf("-- Hehe  --\n");
	return 0;
}
