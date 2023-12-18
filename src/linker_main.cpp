#include <iostream>
#include <fstream>
#include <string.h>
#include <unordered_map>
#include <vector>
#include "../inc/linker.hpp"

using namespace std;

void yyerror(char *s)
{
  cerr << s << endl;
}

int main(int argc, char const *argv[])
{
  const uint8_t place_option_len = 7; // -place=
  const char *file_output = nullptr;
  size_t start_index;
  unordered_map<string, uint32_t> section_places;
  vector<string> input_object_files;
  bool hex_output = false, relocatable_output = false;

  try
  {

    for (int i = 1; i < argc; i++)
    {
      string arg_str(argv[i]);

      // every option should have "continue" at the end of if statement
      if (arg_str == "-o")
      {
        if (i + 1 >= argc)
          throw "Missing output file name";
        file_output = argv[++i];
        continue;
      }

      if ((start_index = arg_str.find("-place=")) != string::npos)
      {
        size_t at_index = arg_str.find("@");
        if (at_index == string::npos)
          throw "Wrong '-place=' parameter format (@ is missing)";

        string section_name = arg_str.substr(start_index + place_option_len, at_index - start_index - place_option_len);
        uint32_t place_value = (uint32_t)stoul(arg_str.substr(at_index + 1));

        if (section_places.find(section_name) != section_places.end())
          throw "Duplicate section entry used with '-place=' option";

        section_places[section_name] = place_value;
        continue;
      }

      if (arg_str == "-hex")
      {
        if (relocatable_output)
          throw "Cannot use both -hex and -relocatable options";

        hex_output = true;
        continue;
      }

      if (arg_str == "-relocatable")
      {
        if (hex_output)
          throw "Cannot use both -hex and -relocatable options";

        relocatable_output = true;
        continue;
      }

      // after all options ==> get input files
      if (arg_str.empty())
        continue;
      input_object_files.push_back(argv[i]);
    }

    if (input_object_files.empty())
    {
      throw "At least one input object file is needed";
      return 1;
    }

    if (!hex_output && !relocatable_output)
    {
      throw "At least one of -hex or -relocatable options must be set";
      return 1;
    }

    // run the linker
    if (file_output)
      Linker::get_instance().set_output_file_name(file_output);

    bool linker_res = Linker::get_instance().load_input_obj_files(input_object_files);

    if (linker_res)
    {
      cout << "Linker finished without error." << endl;
      return 0;
    }
    else
    {
      cout << "Linker finished with an error." << endl;
      return 1;
    }
  }
  catch (const exception &e)
  {
    cout << e.what() << endl;
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

  for (auto iter : section_places)
  {
    cout << "Section: " << iter.first << ", at: " << iter.second << endl;
  }

  for (auto file : input_object_files)
  {
    cout << "Input file: " << file << endl;
  }

  if (file_output)
    cout << "Output file: " << file_output << endl;

  if (hex_output)
    cout << "Output file type: hex" << endl;

  if (relocatable_output)
    cout << "Output file type: relocatable" << endl;

  return 0;
}
