COPTS	= -Wall -c -O2

INSTDIR	= /Applications/Locator

OBJS1	= objs/Locator.o objs/search_thread.o objs/locator_window.o objs/image_button.o objs/util.o
OBJS2	= objs/create_db.o objs/util.o
OBJS3	= objs/Locator.o objs/search_thread.o objs/locator_window.o objs/image_button.o objs/util.o
EXE1	= bin/Locator
EXE2	= bin/create_db
EXE3	= bin/locate

CC	= g++
LL	= g++


all	: dirs $(EXE1) $(EXE2) $(EXE3)

$(EXE1) : $(OBJS1)
	$(LL) $(OBJS1) -lsyllable -lstdc++ -lm -o $@


$(EXE2) : $(OBJS2)
	$(LL) $(OBJS2) -lsyllable -lstdc++ -lm -o $@


$(EXE3) : $(OBJS3)
	$(LL) $(OBJS3) -lsyllable -lstdc++ -lm -o $@

dirs:
	-mkdir objs bin

clean:
	rm -rf bin/* objs/*

install: $(EXE1) $(EXE2) $(EXE3)
	strip --strip-all $(EXE1) $(EXE2) $(EXE3)
	-mkdir $(INSTDIR)
	-mkdir $(INSTDIR)/data
	-mkdir $(INSTDIR)/bin
	-mkdir $(INSTDIR)/icons
	cp -f $(EXE1) $(INSTDIR)/bin
	cp -f $(EXE2) $(INSTDIR)/bin
	cp -f $(EXE3) $(INSTDIR)/bin
	cp -f icons/*.png $(INSTDIR)/icons
	cp -f README $(INSTDIR)
	cp -f COPYING $(INSTDIR)

dist: $(EXE1) $(EXE2) $(EXE3)
	strip --strip-all $(EXE1) $(EXE2) $(EXE3)
	-mkdir $(DIST_DIR)/atheos$(INSTDIR)
	-mkdir $(DIST_DIR)/atheos$(INSTDIR)/data
	-mkdir $(DIST_DIR)/atheos$(INSTDIR)/bin
	-mkdir $(DIST_DIR)/atheos$(INSTDIR)/icons
	cp -f $(EXE1) $(DIST_DIR)/atheos$(INSTDIR)/bin
	cp -f $(EXE2) $(DIST_DIR)/atheos$(INSTDIR)/bin
	cp -f $(EXE3) $(DIST_DIR)/atheos$(INSTDIR)/bin
	cp -f icons/*.png $(DIST_DIR)/atheos$(INSTDIR)/icons
	cp -f README $(DIST_DIR)/atheos$(INSTDIR)
	cp -f COPYING $(DIST_DIR)/atheos$(INSTDIR)

deps:
	-mkdir objs
	-mkdir bin

objs/Locator.o: src/Locator.cpp src/locator.h src/locator_window.h
	$(CC) src/Locator.cpp $(COPTS) -o objs/Locator.o

objs/locator_window.o: src/locator_window.cpp src/locator.h src/locator_window.h
	$(CC) src/locator_window.cpp $(COPTS) -o objs/locator_window.o

objs/search_thread.o: src/search_thread.cpp src/locator.h
	$(CC) src/search_thread.cpp $(COPTS) -o objs/search_thread.o

objs/locator.o: src/Locator.cpp src/locator.h
	$(CC) src/Locator.cpp $(COPTS) -o objs/Locator.o

objs/create_db.o: src/create_db.cpp src/locator.h
	$(CC) src/create_db.cpp $(COPTS) -o objs/create_db.o

objs/image_button.o: src/image_button.cpp src/image_button.h
	$(CC) src/image_button.cpp $(COPTS) -o objs/image_button.o

objs/util.o: src/util.cpp src/util.h
	$(CC) src/util.cpp $(COPTS) -o objs/util.o

