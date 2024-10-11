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
#include "garlic_mem.h"

unsigned int	current_mem_ptr = INI_MEM;

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
	_gs_copiaMem(mem_src, (void *)current_mem_ptr, size);

	printf("Mem start: %x\n", current_mem_ptr);
	current_mem_ptr += size;
	printf("Mem finish: %x\n", current_mem_ptr);
}

static char	store_segments(int n_segments, Elf32_Ehdr *header)
{
	int				i;
	void			*segment_table;
	Elf32_Phdr		*curr_segment;
	unsigned int	mem_src;

	i = 0;
	//print_header(header);
	segment_table = ((void *)header + header->e_phoff);
	while (i < n_segments)
	{
		curr_segment = (Elf32_Phdr *)(segment_table + i * header->e_phentsize);
		//print_segment(curr_segment);
		if (curr_segment->p_type == PT_LOAD && curr_segment->p_filesz != 0)
		{
			mem_src = current_mem_ptr;
			// Obtain the ptr where the segment starts and the size of memory it has to copy.
			if (!copy_memory((void *)header + curr_segment->p_offset, curr_segment->p_filesz))
				return (ERROR);
			// Relocate pointers inside sections using header, theoric starting addr and real starting addr.
			_gm_reubicar((void *)header, curr_segment->p_paddr, (unsigned int *)mem_src);
		}
		i++;
	}
	return (CORRECT);
}

static char	parse_file(FILE *file, Elf32_Buffer *buffer)
{
	struct stat		st;
	int				fd;
	char			*file_data;
	Elf32_Ehdr		*header;

	fd = fileno(file);
	if (fstat(fd, &st) != 0)
		return (ERROR);
	file_data = malloc(st.st_size + 1);
	if (file_data == NULL)
		return (ERROR);
	if (fread(file_data, 1, st.st_size, file) != st.st_size)
	{
		free(file_data);
		return (ERROR);
	}
	*(file_data + st.st_size) = '\0'; // In case I want to print it with printf
	
	header = (Elf32_Ehdr *)file_data;

	if (!store_segments(header->e_phnum, header) == ERROR)
	{
		free(file_data);
		return (ERROR);
	}
	buffer->entry = header->e_entry;
	free(file_data);
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
	char			file_name[20] = "/Programas/XXXX.elf";
	FILE*			file;
	Elf32_Buffer	elf_buffer;

	if (!follows_convention(keyName))
		return ((intFunc) ERROR);

	file_name[11] = *keyName;
	file_name[12] = *(keyName + 1);
	file_name[13] = *(keyName + 2);
	file_name[14] = *(keyName + 3);

	file = fopen(file_name,"rb");
	if (file == NULL)
		return ((intFunc) ERROR);

	if (!parse_file(file, &elf_buffer))
	{
		fclose(file);
		return ((intFunc) ERROR);
	}
	fclose(file);
	return ((intFunc) elf_buffer.entry);
}
