CFLAGS   += -kernel -fno-PIC -c -D__ENABLE_DEBUG__ -ffreestanding
AFLAGS   = -c

C_INCLUDE_PATH=$(ATHEOS_INCLUDE_PATH)

OBJS := hcd.o ehci.o gcc34_memcpy.o


OBJDIR	:= objs
OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))


#
#	rules
#

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o : %.s
	$(CC) $(AFLAGS) -x assembler-with-cpp $< -o $@

$(OBJDIR)/%.o : %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp $< -o $@




all :	objs $(OBJDIR)/usb_ehci

objs:
	mkdir objs

$(OBJDIR)/usb_ehci: $(OBJS) Makefile
	$(CC) -kernel $(OBJS) -o $(OBJDIR)/usb_ehci

install: all
	cp -f $(OBJDIR)/usb_ehci $(IMAGE)/system/drivers/dev/hcd/usb_ehci

dist: install

clean:
	rm -f $(OBJS) $(OBJDIR)/usb_ehci

