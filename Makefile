# --- settings ---

EXE_NAME:=game

# --- Configureations per platform ---

ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM:=PLATFORM_Windows
EXE:=$(EXE_NAME).exe
LIBS:=-lgdi32 -lwinmm -lopengl32 -lxinput
OPTIMIZE_FLAGS:=-MD -Os -s -fno-asynchronous-unwind-tables -fno-tree-loop-distribute-patterns -fno-stack-check -DNOPRINT  -mwindows -Wl,--gc-sections -unwindlib=none -fno-math-errno -fno-unroll-loops -fno-ident -mfancy-math-387 -ffast-math -falign-functions=1 -falign-loops=1 -fmerge-all-constants -ffunction-sections -fdata-sections -fno-stack-protector  #-mno-stack-arg-probe -nostdlib
OPTIMIZED_EXE:=$(EXE_NAME).opt.exe
COMPRESSED_EXE:=$(EXE_NAME).upx.exe
else # OS == Windows_NT
UNAME:=$(shell uname -s)
EXE=$(EXE_NAME)
OPTIMIZE_FLAGS:=-MD -Os -s -fno-asynchronous-unwind-tables -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -ffunction-sections -fdata-sections -Wl,--gc-sections -falign-functions=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfancy-math-387 -ffast-math -unwindlib=none -DNOPRINT -flto -finline-limit=512 -fno-unroll-loops -fno-tree-loop-distribute-patterns 
OPTIMIZED_EXE:=$(EXE_NAME).opt
COMPRESSED_EXE:=$(EXE_NAME).upx
ifeq ($(UNAME),Linux)
LIBS=-lX11 -lm -ludev -lGL -lasound
PLATFORM:=PLATFORM_Linux
endif # UNAME == Linux
endif # OS != Windows_NT
endif # PLATFORM == nil

# --- Dependancies ---

# BROTLI:=-Iproject/brotli/include project/brotli/dec/*.c project/brotli/common/*.c
DEPS:= # $(BROTLI)

# --- Makefile build rules ---

build: $(EXE) .PHONY


$(EXE): src/*.c project/*.c includes/*.h
	$(CC) src/*.c project/*.c $(LIBS) -D$(PLATFORM) -Wall -Wextra -Iincludes $(DEPS) -g -o $@

run: $(EXE) .PHONY
	./$(EXE)

valgrind: $(EXE) .PHONY
	valgrind --leak-check=full -s ./$(EXE)

release: $(OPTIMIZED_EXE) .PHONY

$(OPTIMIZED_EXE): src/*.c project/*.c includes/*.h
	$(CC) src/*.c project/*.c $(LIBS) -D$(PLATFORM) $(OPTIMIZE_FLAGS) -Wall -Wextra -Iincludes $(DEPS) -o $@

compressed: $(COMPRESSED_EXE) .PHONY

$(COMPRESSED_EXE): $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)
	upx --ultra-brute -o$(COMPRESSED_EXE) $(OPTIMIZED_EXE)

size: $(EXE) $(OPTIMIZED_EXE) $(COMPRESSED_EXE) .PHONY
	@echo "Unoptimized Size:" `wc -c < $(EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`
	@echo "Optimized Size:  " `wc -c < $(OPTIMIZED_EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`
	@echo "Compressed Size: " `wc -c < $(COMPRESSED_EXE) | sed ':a;s/\B[0-9]\{3\}\>/,&/;ta'`

project/mino.c: tools/AmalgamateSrc/mino.c
	go run tools/amalgamate.go tools/AmalgamateSrc/mino.c -oproject/mino.c -Iincludes

project/mino.h: tools/AmalgamateSrc/mino.h
	go run tools/amalgamate.go tools/AmalgamateSrc/mino.h -oproject/mino.h -Iincludes 

amalgamation: project/mino.c project/mino.h
	@echo "Amalgamation files built to project folder"

clean: .PHONY
	rm -f $(EXE)
	rm -f $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)

.PHONY: