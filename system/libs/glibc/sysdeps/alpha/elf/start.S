/* Startup code for Alpha/ELF.
   Copyright (C) 1993, 1995, 1996, 1997, 1998 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Richard Henderson <rth@tamu.edu>

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <sysdep.h>

	.text
	.align 3
	.globl _start
	.ent _start, 0
_start:
	.frame fp, 0, zero
	mov	zero, fp
	br	gp, 1f
1:	ldgp	gp, 0(gp)
	subq	sp, 16, sp
	.prologue 0

  /* Load address of the user's main function.  */
	lda	a0, main

	ldl	a1, 16(sp)	/* get argc */
	lda	a2, 24(sp)	/* get argv */

  /* Load address of our own entry points to .fini and .init.  */
	lda	a3, _init
	lda	a4, _fini

  /* Store address of the shared library termination function.  */
	mov	v0, a5

  /* Provide the highest stack address to the user code.  */
	stq	sp, 0(sp)

  /* Call the user's main function, and exit with its value.
     But let the libc call main.  */
	jsr	ra, __libc_start_main

  /* Die very horribly if exit returns.  Call_pal hlt is callable from
     kernel mode only; this will result in an illegal instruction trap.  */
	call_pal 0
	.end _start

/* For ECOFF backwards compatibility. */
weak_alias(_start, __start)
