# --- Directory Paths ---
SRC_unit  =libsrc/libsrc
HEAD_unit =libsrc/headers
COMP_unit =compiled

SRC_mod  =libsrc/libsrc
HEAD_mod =libsrc/headers
COMP_mod =compiled

# --- Dependecies ---
COCOA_INTERFACE =/Users/benjaminrutkowski/Projects/ndGameEngine/Application/Application/interface

# --- Compiling ---
# CXX =/usr/bin/clang++ 
CXX =clang++ 

CXX_FLAGS_unit =-std=c++17 -Wall -fdiagnostics-color=always -c
CXX_INC_unit   =-I$(HEAD_unit) -I$(COCOA_INTERFACE)

CXX_FLAGS_mod =-std=c++17 -Wall -fdiagnostics-color=always -c
CXX_INC_mod   =-I$(HEAD_mod) -I$(COCOA_INTERFACE)

# --- Variables ---
Source_unit  =$(wildcard $(SRC_unit)/*.cpp)
Headers_unit =$(wildcard $(HEAD_unit)/*.hpp)
Objects_unit =$(patsubst $(SRC_unit)/%.cpp,$(COMP_unit)/%.o,$(Source_unit))

Modules_mod =$(patsubst $(SRC_mod)/%/.,%,$(wildcard $(SRC_mod)/*/.))
Source_mod  =$(foreach mod,$(Modules_mod),$(wildcard $(SRC_mod)/$(mod)/*.cpp))
Headers_mod =$(foreach mod,$(Modules_mod),$(wildcard $(HEAD_mod)/$(mod)/*.hpp))
Objects_mod =$(foreach mod,$(Modules_mod),$(patsubst $(SRC_mod)/$(mod)/%.cpp,$(COMP_mod)/%.o,$(wildcard $(SRC_mod)/$(mod)/*.cpp)))

# === Build Targets ===
all: $(Objects_unit) $(Objects_mod)

# --- Compile Units ---
$(Objects_unit): $(COMP_unit)/%.o: $(SRC_unit)/%.cpp $(Headers_unit)
	@echo "--- Compiling <$(patsubst $(COMP_unit)/%.o,%,$@)> Unit ---"
	$(CXX) $(patsubst $(COMP_unit)/%.o,$(SRC_unit)/%.cpp,$@) $(CXX_FLAGS_unit) $(CXX_INC_unit) -o $@

# --- Compile Modules ---
.SECONDEXPANSION:
$(Objects_mod): $(COMP_mod)/%.o: $$(foreach mod,$(Modules_mod),$$(filter $(SRC_mod)/$$(mod)/%.cpp,$(Source_mod))) $(Headers_mod)
	@echo "--- Compiling <$(patsubst $(COMP_mod)/%.o,%,$@)> Module Unit ---"
	$(CXX) $< $(CXX_FLAGS_mod) $(CXX_INC_mod) -o $@

# --- Clean ---
clean:
	@echo "--- Cleaning Library Objects ---"
	@rm -f $(COMP_unit)/*.o

.PHONY: clean all
