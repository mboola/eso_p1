/*------------------------------------------------------------------------------

	"garlic_mem.c" : fase 1 / programador M

	Funciones de carga de un fichero ejecutable en formato ELF, para GARLIC 1.0

------------------------------------------------------------------------------*/
#include <nds.h>
#include <filesystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "garlic_system.h"	// definici�n de funciones y variables de sistema
#include "garlic_mem.h"

unsigned int	current_mem_ptr = INI_MEM;
int offsetSegment = 0;

/* _gm_initFS: inicializa el sistema de ficheros, devolviendo un valor booleano
					para indiciar si dicha inicializaci�n ha tenido �xito;
*/
int _gm_initFS()
{
	return (nitroFSInit(NULL));
}

static int	eso_strlen(char *str)
{
	int	len;

	if (str == NULL)
		return (0);
	len = 0;
	while (*str)
	{
		str++;
		len++;
	}
	return (len);
}

static char	follows_convention(char *str)
{
	int		i;
	char	c;

	if (eso_strlen(str) != 4)
		return (ERROR);
	i = 0;	
	while (i < 4)
	{
		c = *(str + i);
		if (c < 'A' || c > 'Z')
			return (ERROR);
		i++;
	}
	return (CORRECT);
}

static void	print_header(Elf32_Ehdr *header)
{
	printf("Entry point: %x\n", header->e_entry);
	printf("Offset to segments: %d\n", header->e_phoff);
	printf("Offset to sections: %d\n", header->e_shoff);
	printf("Size header: %d\n", header->e_ehsize);
	printf("Size segments: %d\n", header->e_phentsize);
	printf("n entries segments: %d\n", header->e_phnum);
	printf("Size sections: %d\n", header->e_shentsize);
	printf("n entries sections: %d\n", header->e_shnum);
}

static void	print_segment(Elf32_Phdr *segment)
{
	printf("Tipo: %u\n", segment->p_type);
	printf("Offset: %x\n", segment->p_offset);
	printf("Addr: %x\n", segment->p_vaddr);
	printf("Size file: %x\n", segment->p_filesz);
	printf("Size mem: %x\n", segment->p_memsz);
	printf("Flags: %d\n", segment->p_flags);
}

static char	copy_memory(void *mem_src, unsigned int size)
{
	if (current_mem_ptr + size > FINAL_MEM)
		return (ERROR);

	printf("Dir to copy:\n");
	printf("Mem src: %x\n",(const void *) mem_src);
	printf("Mem dst: %x\n", (void *) (current_mem_ptr));
	printf("Size: %d\n", size);
	_gs_copiaMem(mem_src, (void *)current_mem_ptr, size);
	current_mem_ptr += size;
	current_mem_ptr += current_mem_ptr % 4;
	return (CORRECT);
}

static char	copy_segments(char *buffered_file)
{
	Elf32_Ehdr	*header;
	int			i;
	Elf32_Phdr	*curr_segment;

	header = (Elf32_Ehdr *)buffered_file;
	i = 0;
	while (i < header->e_phnum)
	{
		curr_segment = (Elf32_Phdr *)(buffered_file + header->e_phoff + i * header->e_phentsize);
		//print_segment(curr_segment);
		if (curr_segment->p_type == PT_LOAD && curr_segment->p_filesz != 0) //&& curr_segment->p_filesz != 0
		{
			if (!copy_memory((void *)(buffered_file + curr_segment->p_offset), curr_segment->p_memsz))
				return (ERROR);
		}
		i++;
	}
	return (CORRECT);
}

static void	reallocate_mem(char *buffered_file, int initial_mem, Elf32_Addr *e_entry)
{
	Elf32_Phdr	*first_segment;
	Elf32_Ehdr	*header;

	header = (Elf32_Ehdr *)buffered_file;

	// We assume there will only be one segment of code to load.
	// All reallocations will be made based on the first segment loaded.
	first_segment = (Elf32_Phdr *)(buffered_file + header->e_phoff);

	//printf("Dir to rellocate:\n");
	//printf("Mem src: %x\n",(const void *) buffered_file);
	//printf("Mem dst: %x\n", (unsigned int *) (first_segment->p_paddr));
	//printf("Size: %d\n", (unsigned int) initial_mem);
	_gm_reubicar(buffered_file, (unsigned int)(first_segment->p_paddr), (unsigned int *)(initial_mem));

	*e_entry = initial_mem + header->e_entry - first_segment->p_paddr;
}

/*
 *	We read from the .elf in parts to reserve less memory.
 *	Not all file is needed.
 */
static char	parse_file(FILE *file, int *e_entry)
{
	char	*buffered_file;
	int		size;
	int		initial_mem;

	initial_mem = current_mem_ptr;
	if (fseek(file, 0, SEEK_END))
		return (ERROR);
	size = ftell (file);
	if (size == -1)
		return (ERROR);

	buffered_file = malloc(sizeof(char) * size);
	if (buffered_file == NULL)
		return (ERROR);
	if (fseek(file, 0, SEEK_SET))
	{
		free(buffered_file);
		return (ERROR);
	}
	if (fread(buffered_file, sizeof(char), size, file) != size)
	{
		free(buffered_file);
		return (ERROR);
	}
	
	if (copy_segments(buffered_file) == ERROR)
	{
		free(buffered_file);
		return (ERROR);
	}

	reallocate_mem(buffered_file, initial_mem, e_entry);
	
	free(buffered_file);
	return (CORRECT);
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
	char	file_name[20] = "/Programas/XXXX.elf";
	FILE*	file;
	int		e_entry;

	if (!follows_convention(keyName))
		return ((intFunc) ERROR);

	file_name[11] = *keyName;
	file_name[12] = *(keyName + 1);
	file_name[13] = *(keyName + 2);
	file_name[14] = *(keyName + 3);

	file = fopen(file_name,"rb");
	if (file == NULL)
		return ((intFunc) ERROR);

	if (!parse_file(file, &e_entry))
	{
		fclose(file);
		return ((intFunc) ERROR);
	}
	fclose(file);
	return ((intFunc) e_entry);
}
