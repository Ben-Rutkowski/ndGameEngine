# --- Directory Paths ---
DIR_mod =modules
DIR_lib =lib
DIR_app =Application

COMP_mod =modules/compiled
COMP_lib =lib/compiled
COMP_app =Application/compiled

BUILD =build
DEBUG_NAME =debug

# --- Dependencies ---
FRAMEWORKS =-framework AppKit -framework Metal -framework MetalKit -framework QuartzCore
# COCOA_INTERFACE = Application/Application/interface

# --- Compiling ---
CXX =/usr/bin/clang++

CXX_FLAGS =-Wall -fdiagnostics-color=always -fno-objc-arc -g
# CXX_INC   =-I$(COCOA_INTERFACE)
CXX_LINK  =$(FRAMEWORKS)

# --- Variables ---
Objects_mod =$(wildcard $(COMP_mod)/*.o)
Objects_lib =$(wildcard $(COMP_lib)/*.o)
Objects_app =$(wildcard $(COMP_app)/*.o)
Objects_all =$(Objects_mod) $(Objects_lib) $(Objects_app)

# === Build Targets ===
build: module lib app
	@echo "--- Compiling Build ---"
	$(CXX) $(Objects_all) $(CXX_FLAGS) $(CXX_LINK) -o $(BUILD)/$(DEBUG_NAME)

module: 
	@$(MAKE) -C $(DIR_mod)

lib:
	@$(MAKE) -C $(DIR_lib)

app:
	@$(MAKE) -C $(DIR_app)

clean:
	@$(MAKE) -C $(DIR_mod) clean
	@$(MAKE) -C $(DIR_lib) clean
	@$(MAKE) -C $(DIR_app) clean

	@echo "--- Cleaning Build ---"
	@rm -f $(BUILD)/$(DEBUG_NAME)

.PHONY:	build clean module lib app