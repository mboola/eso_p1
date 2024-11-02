#include <nds.h>
#include <filesystem.h>
#include <stdio.h>

FILE	*_ga_fopen(const char * filename, const char * mode)
{
	return (fopen(filename, mode));
}

int _ga_fread(void * buffer, size_t size, size_t numele, FILE * file)
{
	return (fread(buffer, size, numele, file));
}

int _ga_fclose(FILE * file)
{
	return (fclose(file));
}
