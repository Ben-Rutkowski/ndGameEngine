# lib 
# |-- compiled
# |	  |-- lib.a
# |   |-- <unit>.o
# |   `-- ...
# |
# `-- libscr
#     |-- include 
#     |   |-- <unit>.hpp
#     |   |-- header.hpp
#     |   `-- ...
#     |
#     |-- libscr
# 		  |-- <unit>.cpp
# 		  `-- ...

# --- Directory Paths ---
COMPILED =compiled
INCLUDE  =libsrc/include
SRC      =libsrc/libsrc

# --- Packages ---
GLAD.include =/usr/local/glad/include

GLFW.include =`pkg-config --cflags glfw3`

PKG-INCLUDE =-I$(GLAD.include) $(GLFW.include)

# --- Compiling ---
CXX.clang      =/usr/bin/clang++
CXXFLAGS.clang =-std=c++17 -Wall -fdiagnostics-color=always -c -g
INCLUDE.clang  =-I$(CURDIR)/$(INCLUDE) $(PKG-INCLUDE)
COMPILE.clang  =$(foreach src,$(filter %.cpp,$^),$(CURDIR)/$(src))

# --- Units ---
units=$(patsubst $(SRC)/%.cpp,$(COMPILED)/%.o,$(wildcard $(SRC)/*.cpp))

# --- Archive Library ---
$(COMPILED)/lib.a: $(units) $(COMPILED)
	@echo "--- ARCHIVING LIBRARY ---"
	@touch $(units)
	ar rs $(COMPILED)/lib.a $(wildcard $(COMPILED)/*.o)
	@touch $@

# --- Compile Units ---
$(units): $(COMPILED)/%.o: $(SRC)/%.cpp $(wildcard $(INCLUDE)/*.hpp) $(COMPILED)
	@echo "--- COMPILING <$(patsubst $(COMPILED)/%.o,%,$@)> LIBRARY UNIT ---"
	cd $(COMPILED) && $(CXX.clang) $(CXXFLAGS.clang) $(INCLUDE.clang) $(COMPILE.clang)

# --- Create Directories Manully ---
$(COMPILED):
	@mkdir $(COMPILED)

# --- Clean ---
clean:
	@echo "--- CLEANING *.o ---"
	rm $(COMPILED)/*.o
	@echo "-- CLEANING lib.a ---"
	rm $(COMPILED)/lib.a

.PHONY: clean
