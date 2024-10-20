	.arch armv5te
	.eabi_attribute 23, 1
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"OPEN.c"
	.text
	.section	.rodata
	.align	2
.LC0:
	.ascii	"-- Programa OPEN 2 -  PID (%d) --\012\000"
	.align	2
.LC1:
	.ascii	"-- Hehe 3 --\012\000"
	.align	2
.LC2:
	.ascii	"test\000"
	.align	2
.LC3:
	.ascii	"-- Hehe  --\012\000"
	.text
	.align	2
	.global	_start
	.syntax unified
	.arm
	.fpu softvfp
	.type	_start, %function
_start:
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 0, uses_anonymous_args = 0
	str	lr, [sp, #-4]!
	sub	sp, sp, #12
	str	r0, [sp, #4]
	ldr	r3, [sp, #4]
	cmp	r3, #0
	bge	.L2
	mov	r3, #0
	str	r3, [sp, #4]
	b	.L3
.L2:
	ldr	r3, [sp, #4]
	cmp	r3, #3
	ble	.L3
	mov	r3, #3
	str	r3, [sp, #4]
.L3:
	bl	GARLIC_pid
	mov	r3, r0
	mov	r1, r3
	ldr	r0, .L5
	bl	GARLIC_printf
	ldr	r0, .L5+4
	bl	GARLIC_printf
	mov	r1, #0
	ldr	r0, .L5+8
	bl	GARLIC_fopen
	ldr	r0, .L5+12
	bl	GARLIC_printf
	mov	r3, #0
	mov	r0, r3
	add	sp, sp, #12
	@ sp needed
	ldr	pc, [sp], #4
.L6:
	.align	2
.L5:
	.word	.LC0
	.word	.LC1
	.word	.LC2
	.word	.LC3
	.size	_start, .-_start
	.ident	"GCC: (15:10.3-2021.07-4) 10.3.1 20210621 (release)"
