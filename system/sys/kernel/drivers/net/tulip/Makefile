CFLAGS   += -kernel -fno-PIC -c

C_INCLUDE_PATH=$(ATHEOS_INCLUDE_PATH)

OBJS := tulip.o
DRIVER := tulip

OBJDIR	:= objs
OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

$(OBJDIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o : %.cpp
	$(CXX) $(CFLAGS) $< -o $@


$(OBJDIR)/%.o : %.s
	$(CC) $(CFLAGS) -x assembler-with-cpp $< -o $@

$(OBJDIR)/%.o : %.S
	$(CC) $(CFLAGS) -x assembler-with-cpp $< -o $@


all :	objs $(OBJDIR)/tulip

deps:

objs:
	mkdir objs

$(OBJDIR)/tulip: $(OBJS) Makefile
	$(CC) -kernel $(OBJS) -o $(OBJDIR)/$(DRIVER)

install: all
	cp -f $(OBJDIR)/tulip $(IMAGE)/system/drivers/dev/net/eth/tulip

dist: install

clean:
	-rm -f $(OBJS)
	-rm -f $(OBJDIR)/$(DRIVER)

