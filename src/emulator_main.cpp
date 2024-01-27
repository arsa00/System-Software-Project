#include <iostream>
#include <fstream>
#include <string.h>
#include "../inc/emulator.hpp"

using namespace std;

void yyerror(char *s)
{
  cerr << s << endl;
}

int main(int argc, char const *argv[])
{
  const char *file_input = nullptr;

  try
  {

    for (int i = 1; i < argc; i++)
    {
      string arg_str(argv[i]);

      // get input file
      if (arg_str.empty())
        continue;

      file_input = argv[i];
    }

    if (!file_input)
      throw "Emulator did not start, an error occured. { Error: \"Input hex file is missing.\" }";

    // load input hex file
    bool emulator_res = Emulator::get_instance().load_memory_hex_from_file(file_input);
    if (!emulator_res)
    {
      cout << "Emulator did not start, an error occured." << endl;
      return 1;
    }

    // run emulator
    Emulator::get_instance().run();
  }
  catch (const exception &e)
  {
    cout << e.what() << endl;
    return 2;
  }
  catch (string err)
  {
    cout << err << endl;
    return 3;
  }
  catch (char const *err)
  {
    cout << err << endl;
    return 3;
  }

  return 0;
}
