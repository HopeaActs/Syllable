ifeq ($(subdir),csu)
sysdep_routines += errno-loc
endif

ifeq ($(subdir),db2)
CPPFLAGS += -DHAVE_LLSEEK=1
endif

ifeq ($(subdir),malloc)
CFLAGS-malloc.c = -DMORECORE_CLEARS=2
endif

ifeq ($(subdir),misc)
sysdep_routines += sysctl clone llseek umount umount2 readahead \
		   setfsuid setfsgid

sysdep_headers += sys/mount.h sys/acct.h sys/sysctl.h \
		  sys/klog.h sys/kdaemon.h \
		  sys/user.h sys/procfs.h sys/prctl.h \
		  sys/kd.h sys/soundcard.h sys/vt.h \
		  sys/quota.h sys/fsuid.h \
		  scsi/sg.h scsi/scsi.h scsi/scsi_ioctl.h sys/pci.h \
		  sys/ultrasound.h sys/raw.h sys/personality.h sys/epoll.h

install-others += $(inst_includedir)/bits/syscall.h

# Generate the list of SYS_* macros for the system calls (__NR_* macros).
# For bi-arch platforms, the CPU/Makefile defines {32,64}bit-predefine and
# we generate a file that uses <bits/wordsize.h>.
$(objpfx)syscall-%.h $(objpfx)syscall-%.d: ../sysdeps/unix/sysv/linux/sys/syscall.h
	rm -f $(@:.h=.d)-t
	{ \
	 echo '/* Generated at libc build time from kernel syscall list.  */';\
	 echo ''; \
	 echo '#ifndef _SYSCALL_H'; \
	 echo '# error "Never use <bits/syscall.h> directly; include <sys/syscall.h> instead."'; \
	 echo '#endif'; \
	 echo ''; \
	 SUNPRO_DEPENDENCIES='$(@:.h=.d)-t $@' \
	 $(CC) -E -x c $(sysincludes) $< $(addprefix -U,$(64bit-predefine)) \
	   $(addprefix -D,$(32bit-predefine)) -D_LIBC -dM | \
	 sed -n 's@^#define __NR_\([^ ]*\) .*$$@#define SYS_\1 __NR_\1@p' | \
	 LC_ALL=C sort > $(@:.d=.h).new32; \
	 SUNPRO_DEPENDENCIES='$(@:.h=.d)-t $@' \
	 $(CC) -E -x c $(sysincludes) $< $(addprefix -U,$(64bit-predefine)) \
	   $(addprefix -D,$(64bit-predefine)) -D_LIBC -dM | \
	 sed -n 's@^#define __NR_\([^ ]*\) .*$$@#define SYS_\1 __NR_\1@p' | \
	 LC_ALL=C sort > $(@:.d=.h).new64; \
	 if cmp -s $(@:.d=.h).new32 $(@:.d=.h).new64; then \
	   cat $(@:.d=.h).new32; \
	 else \
	   echo '#include <bits/wordsize.h>'; \
	   echo ''; \
	   LC_ALL=C comm -12 $(@:.d=.h).new32 $(@:.d=.h).new64; \
	   echo '#if __WORDSIZE == 64'; \
	   LC_ALL=C comm -13 $(@:.d=.h).new32 $(@:.d=.h).new64; \
	   echo '#else'; \
	   LC_ALL=C comm -23 $(@:.d=.h).new32 $(@:.d=.h).new64; \
	   echo '#endif'; \
	 fi; \
	 rm -f $(@:.d=.h).new32 $(@:.d=.h).new64; \
	} > $(@:.d=.h).new
	mv -f $(@:.d=.h).new $(@:.d=.h)
	sed < $(@:.h=.d)-t > $(@:.h=.d)-t2 \
	    -e 's,$(subst .,\.,$@),$(patsubst $(objpfx)%,$$(objpfx)%,\
					      $(@:.d=.h) $(@:.h=.d)),'
	rm -f $(@:.h=.d)-t
	mv -f $(@:.h=.d)-t2 $(@:.h=.d)


$(inst_includedir)/bits/syscall.h: $(objpfx)syscall-list.h $(+force)
	$(make-target-directory)
	if test -r $@ && cmp -s $< $@; \
        then echo 'bits/syscall.h unchanged'; \
        else $(INSTALL_DATA) $< $@; fi

ifndef no_deps
# Get the generated list of dependencies (probably /usr/include/asm/unistd.h).
-include $(objpfx)syscall-list.d
endif
generated += syscall-list.h syscall-list.d
endif

ifeq ($(subdir),time)
sysdep_headers += sys/timex.h

sysdep_routines += ntp_gettime
endif

ifeq ($(subdir),socket)
sysdep_headers += net/if_ppp.h net/ppp-comp.h \
		  net/ppp_defs.h net/if_arp.h net/route.h net/ethernet.h \
		  net/if_slip.h net/if_packet.h net/if_shaper.h
sysdep_routines += cmsg_nxthdr sa_len
endif

ifeq ($(subdir),sunrpc)
sysdep_headers += nfs/nfs.h
endif

ifeq ($(subdir),termios)
sysdep_headers += termio.h
endif

ifeq ($(subdir),stdio-common)
# Just disable the auto generation in sysdeps/generic
inhibit-siglist := yes
endif

ifeq ($(subdir),posix)
sysdep_headers += bits/initspin.h

sysdep_routines += exit-thread
endif

ifeq ($(subdir),inet)
sysdep_headers += netinet/if_fddi.h netinet/if_tr.h netinet/igmp.h \
		  netipx/ipx.h netash/ash.h netax25/ax25.h netatalk/at.h \
		  netrom/netrom.h netpacket/packet.h netrose/rose.h \
		  neteconet/ec.h
endif

# Don't compile the ctype glue code, since there is no old non-GNU C library.
inhibit-glue = yes

ifeq ($(subdir),dirent)
sysdep_routines += getdirentries getdirentries64
endif

ifeq ($(subdir),nis)
CFLAGS-ypclnt.c = -DUSE_BINDINGDIR=1
endif

ifeq ($(subdir),elf)
sysdep-rtld-routines += dl-brk dl-sbrk

CPPFLAGS-lddlibc4 += -DNOT_IN_libc
endif
