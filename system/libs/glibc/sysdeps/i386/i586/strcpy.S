/* strcpy/stpcpy implementation for i586.
   Copyright (C) 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@cygnus.com>, 1997.

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
#include "asm-syntax.h"

#ifndef USE_AS_STPCPY
# define STRCPY strcpy
#endif

#define magic 0xfefefeff

	.text
ENTRY(STRCPY)
	pushl	%edi
	pushl	%esi
	pushl	%ebx

	movl	16(%esp), %edi
	movl	20(%esp), %esi

	xorl	%eax, %eax
	leal	-1(%esi), %ecx

	movl	$magic, %ebx
	andl	$3, %ecx

#ifdef PIC
	call	2f
2:	popl	%edx
	/* 0xb is the distance between 2: and 1: but we avoid writing
	   1f-2b because the assembler generates worse code.  */
	leal	0xb(%edx,%ecx,8), %ecx
#else
	leal	1f(,%ecx,8), %ecx
#endif

	jmp	*%ecx

	.align 8
1:
	orb	(%esi), %al
	jz	L(end)
	stosb
	xorl	%eax, %eax
	incl	%esi

	orb	(%esi), %al
	jz	L(end)
	stosb
	xorl	%eax, %eax
	incl	%esi

	orb	(%esi), %al
	jz	L(end)
	stosb
	xorl	%eax, %eax
	incl	%esi

L(1):	movl	(%esi), %ecx
	leal	4(%esi),%esi

	subl	%ecx, %eax
	addl	%ebx, %ecx

	decl	%eax
	jnc	L(3)

	movl	%ecx, %edx
	xorl	%ecx, %eax

	subl	%ebx, %edx
	andl	$~magic, %eax

	jne	L(4)

	movl	%edx, (%edi)
	leal	4(%edi),%edi

	jmp	L(1)

L(3):	movl	%ecx, %edx

	subl	%ebx, %edx

L(4):	movb	%dl, (%edi)
	testb	%dl, %dl

	movl	%edx, %eax
	jz	L(end2)

	shrl	$16, %eax
	movb	%dh, 1(%edi)
#ifdef USE_AS_STPCPY
	addl	$1, %edi
#endif

	cmpb	$0, %dh
	jz	L(end2)

#ifdef USE_AS_STPCPY
	movb	%al, 1(%edi)
	addl	$1, %edi

	cmpb	$0, %al
	jz	L(end2)

	addl	$1, %edi
#else
	movb	%al, 2(%edi)
	testb	%al, %al

	leal	3(%edi), %edi
	jz	L(end2)
#endif

L(end):	movb	%ah, (%edi)

L(end2):
#ifdef USE_AS_STPCPY
	movl	%edi, %eax
#endif
	popl	%ebx
	popl	%esi
	popl	%edi
#ifndef USE_AS_STPCPY
	movl	4(%esp), %eax
#endif
	ret
END(STRCPY)
