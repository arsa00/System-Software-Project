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

void yyerror(char *s)
{
  cerr << s << endl;
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

    // cout << file_input << endl;
    yyin = fopen(file_input, "r");
    if (yyin == nullptr)
      throw new file_exception();

    string line;
    ifstream in_file{string(file_input)};
    ofstream out_file{string(file_input) + "_copy.txt"};
    if (in_file && out_file)
    {
      while (getline(in_file, line))
      {
        out_file << line << "\n";
      }
    }
    else
    {
      // Something went wrong
      printf("Cannot read File");
    }
    // Closing file
    in_file.close();
    out_file.close();

    char *file_input_final = (char *)file_input;
    file_input_final = strcat(file_input_final, "_copy.txt");

    yyin = fopen(file_input_final, "r");
    if (yyin == nullptr)
      throw new file_exception();

    // fseek(yyin, 0, SEEK_END);
    // fprintf(yyin, "\n");
    // fseek(yyin, 0, SEEK_SET);

    // yyparse();
    Assembler::get_instance().run(yyin, file_input_final);
  }
  catch (const exception &e)
  {
    cerr << e.what() << endl;
    return 1;
  }

  // test1();
  // test2();
  test3();

  return 0;
}
