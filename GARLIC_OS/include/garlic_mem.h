#ifndef GARLIC_MEM_H
# define GARLIC_MEM_H

# include <sys/stat.h>

# define INI_MEM 0x01004000	// inicio de la ITCM donde cargar los programas
# define FINAL_MEM 0x01008000 // final de la ITCM donde cargar los programas

# define PT_LOAD 1

# define ERROR 0
# define CORRECT 1

typedef unsigned int Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Off;
typedef int Elf32_Sword;
typedef unsigned int Elf32_Word;

#define EI_NIDENT 16

// Cabezera fichero ELF
typedef struct {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Half	e_type;
	Elf32_Half	e_machine;
	Elf32_Word	e_version;
	Elf32_Addr	e_entry; //program entry point. First instruction of start direction
	Elf32_Off	e_phoff; //desplazamiento de la tabla de segmentos (program header),
	Elf32_Off	e_shoff; //desplazamiento de la tabla de secciones (section header),
	Elf32_Word	e_flags;
	Elf32_Half	e_ehsize; //tamaño de este header (creo)
	Elf32_Half	e_phentsize; //tamaño de cada entrada de la tabla de segmentos
	Elf32_Half	e_phnum; //número de entradas de la tabla de segmentos,
	Elf32_Half	e_shentsize; // tamaño de cada entrada de la tabla de secciones,
	Elf32_Half	e_shnum; //número de entradas de la tabla de secciones.
	Elf32_Half	e_shstrndx;
}	Elf32_Ehdr;

// Datos de un segmento
typedef struct {
	Elf32_Word	p_type; //tipo del segmento; solo se cargarán segmentos de tipo 1 (PT_LOAD)
	Elf32_Off	p_offset; //desplazamiento en el fichero del primer byte del segmento,
	Elf32_Addr	p_vaddr;
	Elf32_Addr	p_paddr; //dirección física a cargar el segmento; solo usable como referencia
	Elf32_Word	p_filesz; //tamaño del segmento dentro del fichero
	Elf32_Word	p_memsz; //amaño del segmento dentro de la memoria
	Elf32_Word	p_flags; //contenido es tipo lectura, escritura o ejecutable; o combinacion de las tres
	Elf32_Word	p_align;
}	Elf32_Phdr;

// Datos de una seccion
typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type; //tipo de la sección; las secciones de reubicadores son de tipo 9
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset; //desplazamiento en el fichero del primer byte de la sección,
	Elf32_Word	sh_size; //tamaño de la sección dentro del fichero
	Elf32_Word	sh_link; //índice de la sección que contiene la tabla de símbolos asociada a los reubicadores
	Elf32_Word	sh_info; //índice de la sección sobre la cual se deberán aplicar los reubicadores,
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize; //indica el tamaño en bytes de cada reubicador (típicamente, 8 bytes).
}	Elf32_Shdr;

// Datos importantes de un fichero .elf
typedef struct {
	Elf32_Addr	entry;
	Elf32_Ehdr	header;
	Elf32_Phdr	*segments;
	Elf32_Shdr	*sections;
}	Elf32_Buffer;

// Reubicador
typedef struct {
	Elf32_Addr	r_offset; //dirección de memoria fisica sobre la que hay que aplicar la reubicación
	Elf32_Word	r_info; //Dos partes: 8 bits bajos->tipo de reubicación aplicar; otros->el índice del símbolo sobre aplicar reubicación.
}	Elf32_Rel;

#endif
