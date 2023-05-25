# --- settings ---

EXE_NAME:=game

# --- Configureations per platform ---

ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM:=PLATFORM_Windows
EXE:=$(EXE_NAME).exe
LIBS:=-lgdi32 -lwinmm -lopengl32 -lxinput
OPTIMIZE_FLAGS:=-MD -Os -s -fno-asynchronous-unwind-tables -fno-tree-loop-distribute-patterns -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -ffunction-sections -fdata-sections -Wl,--gc-sections -falign-functions=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfancy-math-387 -ffast-math -unwindlib=none -mwindows -DNOPRINT # -nostdlib
OPTIMIZED_EXE:=$(EXE_NAME).opt.exe
COMPRESSED_EXE:=$(EXE_NAME).upx.exe
else # OS == Windows_NT
UNAME:=$(shell uname -s)
EXE=$(EXE_NAME)
OPTIMIZE_FLAGS:=-MD -Os -s -fno-asynchronous-unwind-tables -fno-tree-loop-distribute-patterns -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -ffunction-sections -fdata-sections -Wl,--gc-sections -falign-functions=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfancy-math-387 -ffast-math -unwindlib=none -DNOPRINT
OPTIMIZED_EXE:=$(EXE_NAME).opt
COMPRESSED_EXE:=$(EXE_NAME).upx
ifeq ($(UNAME),Linux)
LIBS=-lX11 -lm -ludev -lGL -lasound
PLATFORM:=PLATFORM_Linux
endif # UNAME == Linux
endif # OS != Windows_NT
endif # PLATFORM == nil

# --- Makefile build rules ---

build: $(EXE) .PHONY


$(EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -D$(PLATFORM) -Wall -Wextra -Iincludes -g -o $@

run: $(EXE) .PHONY
	./$(EXE)

valgrind: $(EXE) .PHONY
	valgrind --leak-check=full -s ./$(EXE)

release: $(OPTIMIZED_EXE) .PHONY

$(OPTIMIZED_EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -D$(PLATFORM) $(OPTIMIZE_FLAGS) -Wall -Wextra -Iincludes -o $@

compressed: $(COMPRESSED_EXE) .PHONY

$(COMPRESSED_EXE): $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)
	upx --ultra-brute -o$(COMPRESSED_EXE) $(OPTIMIZED_EXE)

size: $(EXE) $(OPTIMIZED_EXE) $(COMPRESSED_EXE) .PHONY
	@echo "Unoptimized Size:" `wc -c < $(EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`
	@echo "Optimized Size:  " `wc -c < $(OPTIMIZED_EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`
	@echo "Compressed Size: " `wc -c < $(COMPRESSED_EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`

clean: .PHONY
	rm -f $(EXE)
	rm -f $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)

.PHONY: