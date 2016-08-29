# Palamedes Makefile

#
# Use with mingw-gcc under linux
# Texlive is needed to build documentation.
#

 #########################
 #                       #
 # To compile for 32 bit #
 #    Set ARCH = 32      #
 #                       #
 #########################


ARCH = 64


ifeq ($(ARCH),64) 
	CC = /usr/bin/x86_64-w64-mingw32-g++
  RC = /usr/bin/x86_64-w64-mingw32-windres
else 
	CC = /usr/bin/i686-pc-mingw32-g++
	RC = /usr/bin/i686-pc-mingw32-windres
endif


OUT_DIR = ../
EXEC = $(OUT_DIR)palamedes.exe
DLL = $(OUT_DIR)hooker.dll
DOCS = $(OUT_DIR)palamedes-gr.pdf $(OUT_DIR)table-gr.pdf
OBJS = palamedes/palamedes.o palamedes/dlg.o 

WARNS = -Wall

# 32-bit
CC = /usr/bin/i686-pc-mingw32-g++
RC = /usr/bin/i686-pc-mingw32-windres

# 64-bit
# CC = /usr/bin/x86_64-w64-mingw32-g++
# RC = /usr/bin/x86_64-w64-mingw32-windres

TEX = xelatex -interaction=nonstopmode -output-directory=$(OUT_DIR) 

# CFLAGS = -O3 -std=c99 -D UNICODE -D _UNICODE -D -D WINVER=0x501 $(WARNS) 
# LDFLAGS = -s -lcomctl32 -Wl,--subsystem,windows
CFLAGS = -O3 -D UNICODE -D _UNICODE -D WINVER=0x0601 $(WARNS) 
LDFLAGS = -s -Wl,--subsystem,windows
all: $(EXEC) $(DOCS)
	$(info ================ Compiled for $(ARCH) bit ==============)

$(EXEC): $(OBJS) $(DLL) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(DLL) -o $@
	
palamedes/palamedes.o: palamedes/palamedes.cpp  
	$(CC) $(FLAGS) -c $< -o $@
	

palamedes/dlg.o: palamedes/dlg.rc
	$(RC)  palamedes/dlg.rc $@

$(DLL): hooker/hooker.o hooker/accentuate.o
	$(CC) $(CFLAGS) -shared $^ -o $@ 
#	-rm $^

hooker/hooker.o: hooker/hooker.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

hooker/accentuate.o: hooker/accentuate.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(DOCS): doc/palamedes-gr.tex doc/table-gr.tex
	-$(TEX) ./doc/palamedes-gr.tex &> /dev/null
	-$(TEX) ./doc/table-gr.tex &> /dev/null
	-$(RM) $(OUT_DIR)*.aux $(OUT_DIR)*.log

clean:
	-$(RM) $(EXEC) $(DLL) $(OBJS) $(DOCS) 
	-$(RM) hooker/hooker.o hooker/accentuate.o
#	-$(RM) *.aux *.log
