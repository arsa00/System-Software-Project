ASSEMBLER_FILES = ./inc/*.hpp ./src/*.cpp

assembler:
	g++ -g -o asembler $(ASSEMBLER_FILES) ./auxiliary/src/lexer.cpp ./auxiliary/src/parser.cpp ./auxiliary/inc/parser.hpp ./auxiliary/inc/exceptions.hpp


clean: 
	rm -f ./auxiliary/inc/*.hpp.gch 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler