# --- settings ---

EXE_NAME:=game

# --- Configureations per platform ---
ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM:=Windows
EXE:=$(EXE_NAME).exe
LIBS:=-lgdi32 -lwinmm -lopengl32
OPTIMIZE_FLAGS:=-Wall -Wextra -MD -Os -s -fno-asynchronous-unwind-tables -fno-tree-loop-distribute-patterns -fno-stack-check -fno-stack-protector -mno-stack-arg-probe -ffunction-sections -fdata-sections -Wl,--gc-sections -falign-functions=1 -falign-loops=1 -fno-math-errno -fno-unroll-loops -fmerge-all-constants -fno-ident -mfancy-math-387 -ffast-math -Wall -unwindlib=none -mwindows -DNOPRINT # -nostdlib
OPTIMIZED_EXE:=$(EXE_NAME).opt.exe
COMPRESSED_EXE:=$(EXE_NAME).upx.exe
else # OS == Windows_NT
UNAME:=$(shell uname -s)
OPTIMIZED_EXE:=$(EXE_NAME).opt
COMPRESSED_EXE:=$(EXE_NAME).upx
ifeq ($(UNAME),Linux)
LIBS=-lX11 -lasound -lm
PLATFORM:=Linux
endif # UNAME == Linux
endif # OS != Windows_NT
endif # PLATFORM == nil

# --- Makefile build rules ---

build: $(EXE) .PHONY


$(EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -DPLATFORM=$(PLATFORM) -std=c99 -Wall -Wextra -Iincludes -o $@

run: $(EXE) .PHONY
	./$(EXE)

release: $(COMPRESSED_EXE) .PHONY

$(OPTIMIZED_EXE): src/*.c includes/*.h
	$(CC) src/*.c $(LIBS) -DPLATFORM=$(PLATFORM) $(OPTIMIZE_FLAGS) -std=c99 -Wall -Wextra -Iincludes -o $@

compressed: $(COMPRESSED_EXE) .PHONY

$(COMPRESSED_EXE): $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)
	upx --ultra-brute -o$(COMPRESSED_EXE) $(OPTIMIZED_EXE)

size: $(EXE) $(OPTIMIZED_EXE) $(COMPRESSED_EXE) .PHONY
	@printf "Unoptimized Size: %'10d\n" `wc -c < $(EXE)`
	@printf "Optimized Size:   %'10d\n" `wc -c < $(OPTIMIZED_EXE)`
	@printf "Compressed Size:  %'10d\n" `wc -c < $(COMPRESSED_EXE)`

clean: .PHONY
	rm -f $(EXE)
	rm -f $(OPTIMIZED_EXE)
	rm -f $(COMPRESSED_EXE)

.PHONY: