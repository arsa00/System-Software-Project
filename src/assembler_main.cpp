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
extern void testObjectFile1();

void yyerror(char *s)
{
  cerr << s << endl;

  Assembler::get_instance().parse_error(s);
}

int main(int argc, char const *argv[])
{
  const char *file_input = nullptr, *file_output = nullptr;
  bool verbose_flag = false;

  try
  {

    for (int i = 1; i < argc; i++)
    {
      string argStr(argv[i]);

      // every option should have "continue" at the end of if statement
      if (argStr == "--verbose") // used for debugging
      {
        verbose_flag = true;
        continue;
      }

      if (argStr == "-o")
      {
        argStr.assign(argv[++i]);
        if (argStr.empty())
          throw "Output file name is not valid";
        file_output = argv[i];
        continue;
      }

      // after all options ==> get input file
      if (argStr.empty())
        throw "Input file name is not valid";
      file_input = argv[i];
    }

    if (file_input == nullptr || file_input == "")
    {
      yyerror("Missing input assembler file");
      return 1;
    }

    // create tempporary copy file with one new line appended (needed because of lexer/parser)
    // char *file_input_copy = new char[strlen(file_input) + 1];
    // strcpy(file_input_copy, file_input);
    // file_input_copy = strcat(file_input_copy, "_copy");

    std::string file_input_copy3(file_input);
    std::size_t pos;
    if ((pos = file_input_copy3.rfind("/")) == std::string::npos)
    {
      if ((pos = file_input_copy3.rfind("\\")) == std::string::npos)
      {
        pos = -1;
      }
    }
    file_input_copy3 = file_input_copy3.substr(++pos) + "_temp_copy_" + to_string(rand());

    // std::cout << file_input_copy3 << std::endl;

    string line;
    ifstream in_file{string(file_input)};
    ofstream copy_file{string(file_input_copy3)};

    if (!in_file)
    {
      // Something went wrong
      yyerror("Cannot read input assembler file");
      return 1;
    }

    if (!copy_file)
    {
      // Something went wrong
      yyerror("Cannot read copy of assembler file");
      return 1;
    }

    while (getline(in_file, line))
    {
      copy_file << line << "\n";
    }

    // Closing files used for copying
    in_file.close();
    copy_file.close();

    // define file handle for parser and lexer
    yyin = fopen(file_input_copy3.c_str(), "r");
    if (yyin == nullptr)
    {
      // Something went wrong
      yyerror("Cannot parse input assembler file");
      remove(file_input_copy3.c_str());
      return 1;
    }

    // set verbose flag
    Assembler::get_instance().verbose_print = verbose_flag;

    // create default output file name
    string default_output_file;
    if(file_output == nullptr)
    {
      while (true)
      {
        default_output_file = "assembler_output_" + to_string(rand());
        ifstream asm_file(default_output_file);
        if (!asm_file.good())
          break;
      }

      if (default_output_file.empty())
        throw "Failed creating output file";
    }

    // run the assembler
    Assembler::get_instance().set_output_file_name(file_output ? string(file_output) : default_output_file);
    bool asmRes = Assembler::get_instance().run();

    // close handle for parser and lexer, and remove the temporary file
    fclose(yyin);
    remove(file_input_copy3.c_str());

    if (asmRes)
    {
      std::cout << "Assembler finished without error." << std::endl;
      return 0;
    }
    else
    {
      std::cout << "Assembler finished with an error." << std::endl;
      return 1;
    }
  }
  catch (const exception &e)
  {
    cerr << e.what() << endl;
    return 1;
  }
  catch (string err)
  {
    cout << err << endl;
    return 2;
  }
  catch (char const *err)
  {
    cout << err << endl;
    return 2;
  }

  // test1();
  // test2();
  // test3();
  // testSymbolJsonRecord1();
  // testSectionJsonRecord1();
  // testObjectFile1();

  return 0;
}
