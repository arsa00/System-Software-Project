

assembler:
	g++ -g -o asembler ./src/assembler.cpp ./auxiliary/src/lexer.cpp ./auxiliary/src/parser.cpp ./auxiliary/inc/parser.hpp ./auxiliary/inc/exceptions.hpp


clean: 
	rm -f ./auxiliary/inc/*.hpp.gch 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler