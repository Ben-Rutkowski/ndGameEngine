# modules 
# |-- compiled
# |   |-- <unit>.o
# |   `-- ...
# |
# `-- src
#     |-- headers 
#     |   |-- <all>.hpp
#     |   `-- ...
#     |
#     |-- src
# 		  |-- main.cpp
# 		  |-- <unit>.cpp
# 		  |
# 		  |-- <module>
# 		  |   |-- <module>.cpp
# 		  |   |-- aux.cpp
# 		  |
# 		  `-- ...

# --- Directory Paths ---
COMPILED =modules/compiled
INCLUDE  =modules/src/headers
SRC      =modules/src/src
LIB      =lib
BUILD    =build

# --- Packages ---
GLAD.include =-I/usr/local/glad/include
GLAD.compile =/usr/local/glad/src/*.c

GLFW.include =`pkg-config --cflags glfw3`
GLFW.library =`pkg-config --libs glfw3`

LIB.include  =-I$(CURDIR)/lib/libsrc/include
LIB.library  =$(CURDIR)/lib/compiled/lib.a

PKG-INCLUDE  =$(GLAD.include) $(GLFW.include) $(LIB.include)
PKG-LIBRARY  =$(GLFW.library) $(LIB.library)
PKG-COMPILE  =$(GLAD.compile)

# --- Compiling ---
CXX.clang      		  =/usr/bin/clang++
CXXFLAGS.object.clang =-std=c++17 -Wall -fdiagnostics-color=always -g -c
CXXFLAGS.app.clang    =-std=c++17 -Wall -fdiagnostics-color=always -g
INCLUDE.clang  		  =-I$(CURDIR)/$(INCLUDE) $(PKG-INCLUDE)
LIBRARY.clang 	      =$(PKG-LIBRARY)
COMPILE.object.clang  =$(foreach unit,$(filter %.cpp,$^),$(CURDIR)/$(unit))
COMPILE.app.clang     =$(wildcard $(COMPILED)/*.o) $() $(PKG-COMPILE)
OUTPUT.clang		  =-o $(CURDIR)/$(BUILD)/app

# --- Units and Modules ---

units   =$(patsubst $(SRC)/%.cpp,$(COMPILED)/%.o,$(wildcard $(SRC)/*.cpp))
modules =$(patsubst $(SRC)/%/.,$(COMPILED)/%.o,$(wildcard $(SRC)/*/.))
 
$(BUILD)/app: $(units) $(modules) $(BUILD) lib
	@echo "--- BUILDING APPLICATION ---"
	@touch $(units) $(modules)
	$(CXX.clang) $(CXXFLAGS.app.clang) $(INCLUDE.clang) $(LIBRARY.clang) $(COMPILE.app.clang) $(OUTPUT.clang)
	@touch $@

# --- Recursively Make lib.a
lib:
	@cd $(LIB) && $(MAKE)

# --- Compile Units ---
$(units): $(COMPILED)/%.o: $(SRC)/%.cpp $(wildcard $(INCLUDE)/*.hpp) $(COMPILED)
	@echo "--- COMPILING <$(patsubst $(COMPILED)/%.o,%,$@)> UNIT ---"
	cd $(COMPILED) && $(CXX.clang) $(CXXFLAGS.object.clang) $(INCLUDE.clang) $(COMPILE.object.clang)

# --- Compile Modules ---
.SECONDEXPANSION:
$(modules): $(COMPILED)/%.o: $$(wildcard $(SRC)/%/*.cpp) $(INCLUDE)/*.hpp $(COMPILED)
	@echo "--- COMPILING <$(patsubst $(COMPILED)/%.o,%,$@)> MODULE ---"
	cd $(COMPILED) && $(CXX.clang) $(CXXFLAGS.object.clang) $(INCLUDE.clang) $(COMPILE.object.clang)

# --- Create Directories Manully ---
$(COMPILED):
	@mkdir $(COMPILED)

$(BUILD):
	@mkdir $(BUILD)

# --- Clean ---
clean:
	@echo "--- CLEANING *.o ---"
	rm $(COMPILED)/*.o
	rm -rf $(BUILD)/*
	@cd $(LIB) && $(MAKE) clean

.PHONY: clean lib all