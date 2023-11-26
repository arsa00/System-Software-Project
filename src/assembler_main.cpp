#include <iostream>
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
    yyin = fopen(file_input, "a+");
    if (yyin == nullptr)
      throw new file_exception();

    fseek(yyin, 0, SEEK_END);
    fprintf(yyin, "\n");
    fseek(yyin, 0, SEEK_SET);

    // yyparse();
    Assembler::get_instance().run(yyin, file_input);
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
