# --- settings ---

EXE_NAME:=game

# --- Configureations per platform ---
ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM:=Windows
EXE:=$(EXE_NAME).exe
LIBS:=-lgdi32 -lwinmm -lopengl32
OPTIMIZE_FLAGS:=-MD -Os -s -fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections -Wl,--gc-sections -falign-functions=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfancy-math-387 -ffast-math -Wall -unwindlib=none -mwindows -DNOPRINT
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

release: $(OPTIMIZED_EXE) .PHONY

$(OPTIMIZED_EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -DPLATFORM=$(PLATFORM) $(OPTIMIZE_FLAGS) -Wall -Iincludes -o $@

size: $(EXE) $(OPTIMIZED_EXE) .PHONY
	@printf "Unoptimized Size: %'10d\n" `wc -c < $(EXE)`
	@printf "Optimized Size:   %'10d\n" `wc -c < $(OPTIMIZED_EXE)`

.PHONY: