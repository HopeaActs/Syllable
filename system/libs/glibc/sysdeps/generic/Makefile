# Copyright (C) 1992, 93, 94, 95, 96, 97 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License
# as published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.

# You should have received a copy of the GNU Library General Public
# License along with the GNU C Library; see the file COPYING.LIB.  If not,
# write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

ifeq (,$(filter-out $(sysdep_dir)/generic/bits $(common-objpfx)/bits,\
	$(dir $(firstword $(wildcard $(+sysdep_dirs:%=%/bits/endian.h))))))

$(common-objpfx)bytesex.h: $(common-objpfx)det_endian
	$(dir $<)$(notdir $<) > $@-tmp
	mv $@-tmp $@

$(common-objpfx)det_endian: $(sysdep_dir)/generic/det_endian.c
	$(common-objdir-compile)

before-compile := $(before-compile) $(common-objpfx)bits/endian.h
common-generated := $(common-generated) bits/endian.h det_endian

endif

ifeq ($(subdir),stdio-common)
ifneq ($(inhibit-siglist),yes)
ifneq ($(cross-compiling),yes)
ifeq	"$(filter %siglist.c,$(before-compile))" ""
before-compile := $(before-compile) $(objpfx)siglist.c
$(objpfx)siglist.c: $(objpfx)make_siglist
	@rm -f $@
	$(dir $<)$(notdir $<) > $@-tmp
	mv $@-tmp $@

make_siglist-CFLAGS = -DSIGNUM_H=\"$(shell pwd)/$(firstword $(wildcard \
				   $(+sysdep_dirs:%=%/bits/signum.h)))\"
$(objpfx)make_siglist: $(sysdep_dir)/generic/make_siglist.c
	$(native-compile)

generated := $(generated) make_siglist siglist.c
endif
endif
endif
endif

ifeq ($(subdir),string)
CFLAGS-wordcopy.c += -Wno-uninitialized
endif
