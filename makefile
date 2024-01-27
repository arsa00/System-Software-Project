INC_FILES = ./inc/*.hpp
INC_FILES := $(filter-out ./inc/emulator.hpp, $(INC_FILES))

SRC_FILES := $(wildcard ./src/*.cpp)
SRC_FILES := $(filter-out ./src/assembler_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/linker_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/emulator_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/emulator.cpp, $(SRC_FILES))

# src and inc files
AUXILIARY_FILES = ./auxiliary/inc/*.hpp ./auxiliary/src/*.cpp

assembler:
	g++ -g -o asembler $(INC_FILES) $(SRC_FILES) $(AUXILIARY_FILES) ./src/assembler_main.cpp

linker:
	rm -f ./linker
	g++ -g -o linker $(INC_FILES) $(SRC_FILES) $(AUXILIARY_FILES) ./src/linker_main.cpp

emulator:
	rm -f ./emulator
	g++ -g -o emulator $(INC_FILES) $(SRC_FILES) $(AUXILIARY_FILES) ./src/emulator_main.cpp ./inc/emulator.hpp ./src/emulator.cpp -lpthread

clean_assembler:
	rm -f ./asembler

clean_linker:
	rm -f ./linker

clean_emulator:
	rm -f ./emulator

clean: 
	rm -f ./auxiliary/inc/*.hpp.gch 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler
	rm -f ./linker
	rm -f ./emulator
