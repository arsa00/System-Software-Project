INC_FILES = ./inc/*.hpp

SRC_FILES := $(wildcard ./src/*.cpp)
SRC_FILES := $(filter-out ./src/assembler_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/linker_main.cpp, $(SRC_FILES))

# src and inc files
AUXILIARY_FILES = ./auxiliary/inc/*.hpp ./auxiliary/src/*.cpp

assembler:
	g++ -g -o asembler $(INC_FILES) $(SRC_FILES) $(AUXILIARY_FILES) ./src/assembler_main.cpp -lpthread

linker:
	rm -f ./linker
	g++ -g -o linker $(INC_FILES) $(SRC_FILES) $(AUXILIARY_FILES) ./src/linker_main.cpp

clean_linker:
	rm -f ./linker

clean: 
	rm -f ./auxiliary/inc/*.hpp.gch 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler
	rm -f ./linker
