ASSEMBLER_FILES = ./inc/*.hpp ./src/*.cpp
AUXILIARY_FILES = ./auxiliary/inc/*.hpp ./auxiliary/src/*.cpp

assembler:
	g++ -g -o asembler $(ASSEMBLER_FILES) $(AUXILIARY_FILES)


clean: 
	rm -f ./auxiliary/inc/*.hpp.gch 
	rm -f ./inc/*.hpp.gch
	rm -f ./asembler