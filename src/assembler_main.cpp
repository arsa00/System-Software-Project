#include <iostream>
#include <fstream>
#include <string.h>
#include "../auxiliary/inc/exceptions.hpp"
#include "../auxiliary/inc/parser.hpp"
#include "../inc/assembler.hpp"

using namespace std;

extern int yyparse();
extern FILE *yyin;

extern void test1();
extern void test2();
extern void test3();
extern void testSymbolJsonRecord1();
extern void testSectionJsonRecord1();

void yyerror(char *s)
{
  cerr << s << endl;

  Assembler::get_instance().parse_error(s);
}

int main(int argc, char const *argv[])
{
  const char *file_input, *file_output;

  try
  {

    for (int i = 1; i < argc; i++)
    {
      string argStr(argv[i]);

      // every option should have "continue" at the end of if statement
      if (argStr == "-o")
      {
        argStr.assign(argv[++i]);
        if (argStr.empty())
          throw new file_exception();
        file_output = argv[i];
        continue;
      }

      // after all options ==> get input file
      if (argStr.empty())
        throw new file_exception();
      file_input = argv[i];
    }

    if (file_input == nullptr || file_input == "")
    {
      yyerror("Missing input assembler file");
      return 1;
    }

    // create tempporary copy file with one new line appended (needed because of lexer/parser)
    char *file_input_copy = new char[strlen(file_input) + 1];
    strcpy(file_input_copy, file_input);
    file_input_copy = strcat(file_input_copy, "_copy");

    string line;
    ifstream in_file{string(file_input)};
    ofstream copy_file{string(file_input_copy)};

    if (!in_file)
    {
      // Something went wrong
      yyerror("Cannot read input assembler file1");
      return 1;
    }

    if (!copy_file)
    {
      // Something went wrong
      yyerror("Cannot read copy of assembler file");
      return 1;
    }

    if (in_file && copy_file)
    {
      while (getline(in_file, line))
      {
        copy_file << line << "\n";
      }
    }
    else
    {
      // Something went wrong
      yyerror("Cannot read input assembler file");
      return 1;
    }

    // Closing files used for copying
    in_file.close();
    copy_file.close();

    // define file handle for parser and lexer
    yyin = fopen(file_input_copy, "r");
    if (yyin == nullptr)
    {
      // Something went wrong
      yyerror("Cannot parse input assembler file");
      return 1;
    }

    // run the assembler
    Assembler::get_instance().run();

    // close handle for parser and lexer, and remove the temporary file
    fclose(yyin);
    remove(file_input_copy);
  }
  catch (const exception &e)
  {
    cerr << e.what() << endl;
    return 1;
  }

  // test1();
  // test2();
  // test3();
  testSymbolJsonRecord1();
  testSectionJsonRecord1();

  return 0;
}
