/* Startup code for elf64-sparc
   Copyright (C) 1997, 1998 Free Software Foundation, Inc.
   Contributed by Richard Henderson <richard@gnu.ai.mit.edu>, 1997.

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


	.section ".text"
	.align 4
	.global _start
	.type _start,#function
_start:

  /* Terminate the stack frame, and reserve space for functions to
     drop their arguments.  */
	mov	%g0, %fp
	sub	%sp, 6*8, %sp

  /* Extract the arguments and environment as encoded on the stack.  The
     argument info starts after one register window (16 words) past the SP,
     plus the bias we added, plus the magic v9 STACK_BIAS.  */
	ldx	[%sp+STACK_BIAS+22*8], %o1
	add	%sp, STACK_BIAS+23*8, %o2

  /* Load the addresses of the user entry points.  */
        sethi   %hi(main), %o0
        sethi   %hi(_init), %o3
        sethi   %hi(_fini), %o4
        or      %o0, %lo(main), %o0
        or      %o3, %lo(_init), %o3
        or      %o4, %lo(_fini), %o4

  /* When starting a binary via the dynamic linker, %g1 contains the
     address of the shared library termination function, which will be
     registered with atexit().  If we are statically linked, this will
     be NULL.  */
	mov     %g1, %o5

  /* Let libc do the rest of the initialization, and call main.  */
	call    __libc_start_main
	 nop

  /* Die very horribly if exit returns.  */
	illtrap	0

	.size _start, .-_start
