# Copyright (C) 1997, 1998 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.

# You should have received a copy of the GNU Library General Public
# License along with the GNU C Library; see the file COPYING.LIB.  If not,
# write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

#
#	Sub-makefile for hesiod portion of the library.
#
subdir	:= hesiod

distribute := hesiod.h hesiod_p.h README.hesiod

extra-libs := libnss_hesiod
extra-libs-others = $(extra-libs)

subdir-dirs = nss_hesiod
vpath %.c nss_hesiod

libnss_hesiod-routines	:= hesiod hesiod-grp hesiod-pwd hesiod-service
# Build only shared library
libnss_hesiod-inhibit-o	= $(filter-out .os,$(object-suffixes))

include ../Rules

CFLAGS-hesiod.c = -DSYSCONFDIR='"$(sysconfdir)"'

# Depend on libc.so so a DT_NEEDED is generated in the shared objects.
# This ensures they will load libc.so for needed symbols if loaded by
# a statically-linked program that hasn't already loaded it.
# The Hesiod NSS modules also needs the resolver and some help from
# the file service.
$(objpfx)libnss_hesiod.so: $(common-objpfx)resolv/libresolv.so \
                           $(common-objpfx)nss/libnss_files.so \
                           $(common-objpfx)libc.so
