# Palamedes Makefile
#
# Use with mingw-gcc under linux
# Need texlive to build docs.
#
EXEC = palamedes.exe
DLL = hooker.dll
DOCS = palamedes-gr.pdf table-gr.pdf
OBJS = palamedes/palamedes.o palamedes/dlg.o 

WARNS = -Wall
CC = /usr/bin/i686-pc-mingw32-g++
TEX = xelatex -interaction=nonstopmode 

# CFLAGS = -O3 -std=c99 -D UNICODE -D _UNICODE -D -D WINVER=0x500 $(WARNS) 
# LDFLAGS = -s -lcomctl32 -Wl,--subsystem,windows
CFLAGS = -O3 -D UNICODE -D _UNICODE -D WINVER=0x501 $(WARNS) 
LDFLAGS = -s -Wl,--subsystem,windows
RC = /usr/bin/i686-pc-mingw32-windres

all: $(EXEC) $(DOCS)

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
	-rm *.aux *.log

clean:
	-rm $(EXEC) $(DLL) $(OBJS) $(DOCS) 
	-rm hooker/hooker.o hooker/accentuate.o
#	-rm *.aux *.log
