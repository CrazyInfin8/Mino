# --- settings ---

EXE_NAME:=game

# --- Configureations per platform ---
ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM:=Windows
EXE:=$(EXE_NAME).exe
LIBS:=-lgdi32 -lwinmm
OPTIMIZED_EXE:=$(EXE_NAME).opt.exe
else # OS == Windows_NT
UNAME:=$(shell uname -s)
OPTIMIZED_EXE:=$(EXE_NAME).opt.exe
ifeq ($(UNAME),Linux)
LIBS=-lX11 -lasound -lm
PLATFORM:=Linux
endif # UNAME == Linux
endif # OS != Windows_NT
endif # PLATFORM == nil

# --- Makefile build rules ---

build: $(EXE) .PHONY

$(EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -DPLATFORM=$(PLATFORM) -Wall -Iincludes -o $@

run: $(EXE) .PHONY
	./$(EXE)


.PHONY: