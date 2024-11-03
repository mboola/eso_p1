/*------------------------------------------------------------------------------

	"DOTP.c" : Producte escalar de vector aleatoris [-10..10]
	de longitud 1+2^arg

------------------------------------------------------------------------------*/

#include <GARLIC_API.h>

static unsigned char	calculate_length(int arg)
{
	unsigned char	length;

	if (arg == 0)
		return (2);
	length = 1;
	for (int i = 1; i <= arg; i++)
	{
		length *= 2;
	}
	return (length + 1);
}


static void	generate_numbers(int *vector, unsigned char length)
{
	unsigned int result, x;

	for (int i = 0; i < length; i++)
	{
		GARLIC_divmod(GARLIC_random(), 21, &x, &result);
		vector[i] = result - 10;
	}	
}

static void	print_vector(int *vector, unsigned char length, char *str)
{
	GARLIC_printf("%s: (", str);
	for (int i = 0; i < length - 1; i++)
	{
		GARLIC_printf("%d, ", vector[i]);
	}
	GARLIC_printf("%d)\n", vector[length - 1]);
}

static int	scalar_product(int *vector1, int *vector2, unsigned char length)
{
	int	product;

	product = 0;
	for (int i = 0; i < length; i++)
	{
		product += vector1[i] * vector2[i];
	}
	return (product);
}

int _start(int arg)
{
	unsigned char	length;
	int				vector1[9];
	int				vector2[9];
	int				product;
	
	if (arg < 0)
		arg = 0;
	else if (arg > 3)
		arg = 3;

	GARLIC_printf("-- Programa DOTP  -  PID (%d) --\n", GARLIC_pid());
	
	length = calculate_length(arg);

	generate_numbers(vector1, length);
	print_vector(vector1, length, "Vector1");
	generate_numbers(vector2, length);
	print_vector(vector2, length, "Vector2");

	product = scalar_product(vector1, vector2, length);

	GARLIC_printf("-- Result (%d) --\n", product);
	return (0);
}
