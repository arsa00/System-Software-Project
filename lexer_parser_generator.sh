bison -o ./src/parser.cpp --defines=./inc/parser.hpp ./misc/parser.y &&
flex -o ./src/lexer.cpp ./misc/lexer.l 