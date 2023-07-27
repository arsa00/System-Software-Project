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
    INSTRUCTION_HALT = 258,
    INSTRUCTION_INT = 259,
    INSTRUCTION_IRET = 260,
    INSTRUCTION_CALL = 261,
    INSTRUCTION_RET = 262,
    INSTRUCTION_JMP = 263,
    INSTRUCTION_BEQ = 264,
    INSTRUCTION_BNE = 265,
    INSTRUCTION_BGT = 266,
    INSTRUCTION_PUSH = 267,
    INSTRUCTION_POP = 268,
    INSTRUCTION_XCHG = 269,
    INSTRUCTION_ADD = 270,
    INSTRUCTION_SUB = 271,
    INSTRUCTION_MUL = 272,
    INSTRUCTION_DIV = 273,
    INSTRUCTION_NOT = 274,
    INSTRUCTION_AND = 275,
    INSTRUCTION_OR = 276,
    INSTRUCTION_XOR = 277,
    INSTRUCTION_SHL = 278,
    INSTRUCTION_SHR = 279,
    INSTRUCTION_LD = 280,
    INSTRUCTION_ST = 281,
    INSTRUCTION_CSRRD = 282,
    INSTRUCTION_CSRWR = 283,
    DIRECTIVE_GLOBAL = 284,
    DIRECTIVE_EXTERN = 285,
    DIRECTIVE_SECTION = 286,
    DIRECTIVE_WORD = 287,
    DIRECTIVE_SKIP = 288,
    DIRECTIVE_ASCII = 289,
    DIRECTIVE_EQU = 290,
    DIRECTIVE_END = 291,
    LABEL = 292,
    SYMBOL = 293,
    LITERAL_BIN = 294,
    LITERAL_OCT = 295,
    LITERAL_DEC = 296,
    LITERAL_HEX = 297,
    LITERAL_STRING = 298,
    COMMA = 299,
    EOL = 300,
    UNDEFIEND = 301
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 9 "./auxiliary/parser.y"

  char *symbol_name;
  char *literal_value;

#line 109 "./auxiliary/inc/parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_AUXILIARY_INC_PARSER_HPP_INCLUDED  */
