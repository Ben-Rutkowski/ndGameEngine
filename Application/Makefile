# --- Directory Paths ---
SRC_inter  =Application/interface
HEAD_inter =Application/interface
COMP_inter =compiled

SRC_app   =Application/src
HEAD_app  =Application/src
COMP_app  =compiled

SRC_draw  =Application/src/draw_routines
COMP_draw =compiled

SRC_shdr  =Application/shaders
COMP_shdr =compiled/shader_objects
LIB_shdr  =compiled/shader_lib

MTL_LIB  =compiled/shader_lib/MetalLibrary.metallib

# --- Compiling ---
# CXX   =/usr/bin/clang++
# CC    =/usr/bin/clang
# XCODE =/usr/bin/xcrun

CXX   =clang++
CC    =clang
XCODE =xcrun

OBJC_FLAGS_app =-Wall -c -fdiagnostics-color=always -fno-objc-arc
OBJC_INC_app   =-I$(HEAD_app) -I$(HEAD_inter)

OBJC_FLAGS_draw =-Wall -c -fdiagnostics-color=always -fno-objc-arc
OBJC_INC_draw   =-I$(HEAD_app) -I$(HEAD_inter)

MTL_FLAGS_shdr =-sdk macosx metal -c
MTL_FLAGS_lib  =-sdk macosx metallib
MTL_INC_shdr   =-I$(HEAD_inter)

CXX_FLAGS_inter =-Wall -c -fdiagnostics-color=always -fno-objc-arc -g
CXX_INC_inter   =-I$(HEAD_app) -I$(HEAD_inter)

# --- Variables ---
Source_app  =$(wildcard $(SRC_app)/*.m)
Headers_app =$(wildcard $(HEAD_app)/*.h)
Objects_app =$(patsubst $(SRC_app)/%.m,$(COMP_app)/%.o,$(Source_app))

Source_draw  =$(wildcard $(SRC_draw)/*.m)
Objects_draw =$(patsubst $(SRC_draw)/%.m,$(COMP_draw)/%.o,$(Source_draw))

Source_shdr  =$(wildcard $(SRC_shdr)/*.metal)
Objects_shdr =$(patsubst $(SRC_shdr)/%.metal,$(COMP_shdr)/%.air,$(Source_shdr))

Source_inter  =$(wildcard $(SRC_inter)/*.mm)
Headers_inter =$(wildcard $(HEAD_inter)/*.h) $(wildcard $(HEAD_inter)/*.hpp)
Objects_inter =$(patsubst $(SRC_inter)/%.mm,$(COMP_inter)/%.o,$(Source_inter))

# === Build Targets ===
build: $(Objects_app) $(Objects_inter) $(Objects_draw) shaders

shaders: $(Objects_shdr)
ifeq ($(strip $(Objects_shdr)),)
else 
	@echo "--- Compiling Shader Library ---"
	$(XCODE) $(MTL_FLAGS_lib) $^ -o $(MTL_LIB)
endif

# --- App Targets ---
$(Objects_app): $(COMP_app)/%.o: $(SRC_app)/%.m $(Headers_app) $(Headers_inter)
	@echo "--- Compiling <$(patsubst $(COMP_app)/%.o,%,$@)> App Unit ---"
	$(CC) $(patsubst $(COMP_app)/%.o,$(SRC_app)/%.m,$@) $(OBJC_FLAGS_app) $(OBJC_INC_app) -o $@

$(Objects_inter): $(COMP_inter)/%.o: $(SRC_inter)/%.mm $(Headers_app) $(Headers_inter)
	@echo "--- Compiling <$(patsubst $(COMP_inter)/%.o,%,$@)> Interface Unit ---"
	$(CXX) $(patsubst $(COMP_inter)/%.o,$(SRC_inter)/%.mm,$@) $(CXX_FLAGS_inter) $(CXX_INC_inter) -o $@

$(Objects_draw): $(COMP_draw)/%.o: $(SRC_draw)/%.m $(Headers_app) $(Headers_inter)
	@echo "--- Compiling <$(patsubst $(COMP_draw)/%.o,%,$@)> Draw Routine Unit ---"
	$(CXX) $(patsubst $(COMP_draw)/%.o,$(SRC_draw)/%.m,$@) $(OBJC_FLAGS_draw) $(OBJC_INC_draw) -o $@

# --- Shader Targets ---
$(Objects_shdr): $(COMP_shdr)/%.air: $(SRC_shdr)/%.metal $(Headers_inter)
	@echo "--- Compiling <$(patsubst $(COMP_shdr)/%.air,%,$@)> Shader Unit ---"
	$(XCODE) $(MTL_FLAGS_shdr) $(patsubst $(COMP_shdr)/%.air,$(SRC_shdr)/%.metal,$@) $(MTL_INC_shdr) -o $@ 

# --- Extra Targets ---
clean:
	@echo "--- Cleaning App Objects ---"
	@rm -f $(COMP_app)/*.o

	@echo "--- Cleaning Interface Objects ---"
	@rm -f $(COMP_inter)/*.o

	@echo "--- Cleaning Shader Objects ---"
	@rm -f $(COMP_shdr)/*.air

	@echo "--- Cleaning Shader Library ---"
	@rm -f $(LIB_shdr)/*.metallib

.PHONY: clean build binary shaders
.SECONDEXPANSION:
