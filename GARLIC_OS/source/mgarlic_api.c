#include <stdio.h>

FILE	*_ga_fopen(const char * filename, const char * mode)
{
    printf("Read filename\n");
    return 0;
}

int _ga_fread(void * buffer, size_t size, size_t numele, FILE * file)
{
    return 0;
}

int _ga_fclose(FILE * file)
{
    return 0;
}
