INC_FILES = ./inc/*.hpp
INC_FILES := $(filter-out ./inc/emulator.hpp, $(INC_FILES))

SRC_FILES := $(wildcard ./src/*.cpp)
SRC_FILES := $(filter-out ./src/assembler_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/linker_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/emulator_main.cpp, $(SRC_FILES))
SRC_FILES := $(filter-out ./src/emulator.cpp, $(SRC_FILES))

# src and inc files
# MISC_FILES = ./misc/inc/*.hpp ./misc/src/*.cpp

assembler:
	g++ -g -o asembler $(INC_FILES) $(SRC_FILES) ./src/assembler_main.cpp

linker:
	rm -f ./linker
	g++ -g -o linker $(INC_FILES) $(SRC_FILES) ./src/linker_main.cpp

emulator:
	rm -f ./emulator
	g++ -g -o emulator $(INC_FILES) $(SRC_FILES) ./src/emulator_main.cpp ./inc/emulator.hpp ./src/emulator.cpp -lpthread

lexer_and_parser:
	sh ./lexer_parser_generator.sh

clean_assembler:
	rm -f ./asembler

clean_linker:
	rm -f ./linker

clean_emulator:
	rm -f ./emulator

clean_lexer_and_parser:
	rm -f ./inc/parser.hpp
	rm -f ./src/parser.cpp
	rm -f ./src/lexer.cpp

clean_all: 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler
	rm -f ./linker
	rm -f ./emulator
	rm -f ./inc/parser.hpp
	rm -f ./src/parser.cpp
	rm -f ./src/lexer.cpp
