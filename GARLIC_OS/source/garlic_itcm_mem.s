@;==============================================================================
@;
@;	"garlic_itcm_mem.s":	c�digo de rutinas de soporte a la carga de
@;							programas en memoria (version 1.0)
@;
@;==============================================================================

.section .itcm,"ax",%progbits

	.arm
	.align 2
	

	.global _gm_reubicar
	@; rutina para interpretar los 'relocs' de un fichero ELF y ajustar las
	@; direcciones de memoria correspondientes a las referencias de tipo
	@; R_ARM_ABS32, restando la direcci�n de inicio de segmento y sumando
	@; la direcci�n de destino en la memoria;
	@;Par�metros:
	@; R0: direcci�n inicial del buffer de fichero (char *fileBuf)
	@; R1: direcci�n de inicio de segmento (unsigned int pAddr)
	@; R2: direcci�n de destino en la memoria (unsigned int *dest)
	@;Resultado:
	@; cambio de las direcciones de memoria que se tienen que ajustar
_gm_reubicar:
	push {lr}

	@; Buscamos la seccion de reubicadores
	@; Para cada seccion de "R_ARM_ABS32" obtenemos:
		@; Obtener direccion mem dst de la posicion a reubicar
		@; Aplicar direccion a mem src del reubicador


	pop {pc}


.end

