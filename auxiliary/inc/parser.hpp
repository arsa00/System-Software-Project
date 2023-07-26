/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_AUXILIARY_INC_PARSER_HPP_INCLUDED
# define YY_YY_AUXILIARY_INC_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INSTRUCTION_ZERO_PARAM = 258,
    INSTRUCTION_ONE_PARAM = 259,
    INSTRUCTION_TWO_PARAM = 260,
    INSTRUCTION_THREE_PARAM = 261,
    DIRECTIVE_GLOBAL = 262,
    DIRECTIVE_EXTERN = 263,
    DIRECTIVE_SECTION = 264,
    DIRECTIVE_WORD = 265,
    DIRECTIVE_SKIP = 266,
    DIRECTIVE_ASCII = 267,
    DIRECTIVE_EQU = 268,
    DIRECTIVE_END = 269,
    SYMBOL = 270,
    LITERAL_BIN = 271,
    LITERAL_OCT = 272,
    LITERAL_DEC = 273,
    LITERAL_HEX = 274,
    WHITESPACE = 275,
    COMMA = 276,
    EOL = 277,
    UNDEFIEND = 278
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "./auxiliary/parser.y"

  char *alias;
  char *symbol_name;
  char *literal_value;

#line 87 "./auxiliary/inc/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_AUXILIARY_INC_PARSER_HPP_INCLUDED  */
