/*
 * Written by Ulrich Drepper <drepper@cygnus.com>.
 * Based on e_remainder by J.T. Conklin <jtc@netbsd.org>.
 * Public domain.
 */

#include <machine/asm.h>

ENTRY(__remquof)
	flds	8(%esp)
	flds	4(%esp)
1:	fprem1
	fstsw	%ax
	sahf
	jp	1b
	fstp	%st(1)
	/* Compute the congruent of the quotient.  */
	movl	%eax, %ecx
	shrl	$8, %eax
	shrl	$12, %ecx
	andl	$4, %ecx
	andl	$3, %eax
	orl	%eax, %ecx
	movl	$0xef2960, %eax
	shrl	%cl, %eax
	andl	$3, %eax
	movl	12(%esp), %ecx
	movl	4(%esp), %edx
	xorl	8(%esp), %edx
	testl	$0x80000000, %edx
	jz	1f
	negl	%eax
1:	movl	%eax, (%ecx)
	ret
END (__remquof)
weak_alias (__remquof, remquof)
