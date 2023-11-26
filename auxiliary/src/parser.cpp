/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "./auxiliary/parser.y"

  #include <iostream>
  #include <string>
  #include "../../inc/all_instructions.hpp"
  #include "../../inc/all_directives.hpp"
  #include "../../inc/symbol.hpp"
  #include "../../inc/literal.hpp"
  #include "../../inc/command_builder.hpp"
   #include "../../inc/assembler.hpp"

  extern int yylex (void);
  extern void yyerror(char* s);

  void build_instruction(type::INSTRUCTION_TYPE ins_type)
  {
    if (!Assembler::get_instance().get_running_flag())
      return;

    Instruction *ins = CommandBuilder::get_instance().build_instruction(ins_type);
    if(ins) 
      Assembler::get_instance().add_command(ins);
  }

  void build_directive(type::DIRECTIVE_TYPE dir_type)
  {
    if (!Assembler::get_instance().get_running_flag())
      return;
      
    Directive *dir = CommandBuilder::get_instance().build_directive(dir_type);
    if(dir) 
      Assembler::get_instance().add_command(dir);
  }

#line 104 "./auxiliary/src/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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
    GP_REG = 299,
    CS_REG = 300,
    PLUS = 301,
    SQUARE_BRACKET_L = 302,
    SQUARE_BRACKET_R = 303,
    DOLLAR = 304,
    COMMA = 305,
    EOL = 306,
    UNDEFIEND = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 36 "./auxiliary/parser.y"

  char *symbol_name;
  char *literal_value;
  int gpr_index;
  int csr_index;

#line 216 "./auxiliary/src/parser.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_AUXILIARY_INC_PARSER_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  89
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  11
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  180

#define YYUNDEFTOK  2
#define YYMAXUTOK   307


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   104,   104,   105,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   152,   153,   157,   158,   159,   163,
     167,   168,   169,   170,   174,   178,   182,   186,   187,   188,
     189,   190,   191,   192,   193
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INSTRUCTION_HALT", "INSTRUCTION_INT",
  "INSTRUCTION_IRET", "INSTRUCTION_CALL", "INSTRUCTION_RET",
  "INSTRUCTION_JMP", "INSTRUCTION_BEQ", "INSTRUCTION_BNE",
  "INSTRUCTION_BGT", "INSTRUCTION_PUSH", "INSTRUCTION_POP",
  "INSTRUCTION_XCHG", "INSTRUCTION_ADD", "INSTRUCTION_SUB",
  "INSTRUCTION_MUL", "INSTRUCTION_DIV", "INSTRUCTION_NOT",
  "INSTRUCTION_AND", "INSTRUCTION_OR", "INSTRUCTION_XOR",
  "INSTRUCTION_SHL", "INSTRUCTION_SHR", "INSTRUCTION_LD", "INSTRUCTION_ST",
  "INSTRUCTION_CSRRD", "INSTRUCTION_CSRWR", "DIRECTIVE_GLOBAL",
  "DIRECTIVE_EXTERN", "DIRECTIVE_SECTION", "DIRECTIVE_WORD",
  "DIRECTIVE_SKIP", "DIRECTIVE_ASCII", "DIRECTIVE_EQU", "DIRECTIVE_END",
  "LABEL", "SYMBOL", "LITERAL_BIN", "LITERAL_OCT", "LITERAL_DEC",
  "LITERAL_HEX", "LITERAL_STRING", "GP_REG", "CS_REG", "PLUS",
  "SQUARE_BRACKET_L", "SQUARE_BRACKET_R", "DOLLAR", "COMMA", "EOL",
  "UNDEFIEND", "$accept", "code", "line", "symbol_list",
  "symbol_literal_list", "single_symbol", "all_num_literals",
  "string_literal", "single_gp_reg", "single_cs_reg", "ld_st_operand", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307
};
# endif

#define YYPACT_NINF (-77)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     176,   -47,   -37,   -33,   -18,   -12,   -18,   -28,   -28,   -28,
     -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,   -28,
     -28,   -28,   -28,   -32,   -28,    -5,   -28,     3,     3,     3,
     -18,    -8,     0,    -9,   -77,   -77,    44,   176,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,    -6,    -3,   -77,    -2,
       1,   -77,     4,     5,     6,     2,     7,     9,    10,    11,
      12,    13,    15,    14,    17,    35,    37,    39,   -28,   -18,
     -77,   -77,   -77,    40,    42,   -77,    43,    45,   -23,   -77,
     -15,    46,   -13,   -77,   -77,    47,   -77,    48,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -28,   -28,   -28,   -77,   -77,
     -28,   -28,   -28,   -28,   -28,   -77,   -28,   -28,   -28,   -28,
     -28,   -35,   -77,   -77,   -28,   -32,   -28,    -5,     3,   -77,
     -77,   -77,   -18,   -77,   -77,   -77,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    61,    63,    64,   -18,
     -77,    65,    66,    68,    69,    72,    73,   -18,   -18,   -18,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
      76,    77,   -77,   -77,   -77,   -77,    75,    78,    80,    81,
      82,    83,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    11,    43,     0,     2,    12,    13,
      14,    49,    50,    51,    52,    53,     0,     0,    15,     0,
       0,    55,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      60,    59,    61,     0,     0,    56,     0,     0,     0,    45,
       0,     0,     0,    47,    48,     0,    54,     0,     9,     1,
       3,    16,    17,    18,    19,     0,     0,     0,    26,    27,
       0,     0,     0,     0,     0,    33,     0,     0,     0,     0,
       0,     0,    58,    57,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     4,     8,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      62,     0,     0,     0,     0,    44,    46,     0,     0,     0,
      28,    29,    30,    31,    32,    34,    35,    36,    37,    38,
       0,     0,    41,    42,    39,    40,     0,     0,     0,     0,
       0,     0,    64,    63,    20,    21,    22,    23,    24,    25
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -77,    20,   -77,   -27,   -76,    -4,    -1,   -77,    62,   -70,
     -65
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    36,    37,    78,    82,    79,    71,    87,    72,    76,
      73
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      46,    80,    49,    47,    38,    50,    41,    42,    43,    44,
      45,   139,    51,   140,    39,    68,    51,    69,    40,    70,
      41,    42,    43,    44,    45,    81,    83,   118,   119,    84,
      85,    42,    43,    44,    45,   118,   120,   122,   123,    48,
      75,    41,    88,    86,    89,    91,   146,   144,    92,    93,
     142,     0,    94,    98,    95,    96,    97,    90,    99,   100,
     101,   102,   103,   104,   106,   112,   105,   107,   113,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,   108,    74,   109,    77,   110,
     114,   145,   115,   116,     0,   117,     0,   121,   124,   125,
     147,   148,   149,     0,   150,   151,   152,   153,   154,   155,
     156,    70,   157,     0,   158,   159,   162,   163,    83,   164,
     165,    84,   118,   122,   172,   173,   174,     0,     0,   175,
     111,   176,   177,   178,   179,   160,     0,     0,   161,     0,
       0,     0,     0,   166,   168,   170,   167,   169,   171,     0,
       0,     0,     0,     0,     0,     0,     0,   126,   127,   128,
       0,     0,   129,   130,   131,   132,   133,     0,   134,   135,
     136,   137,   138,     0,     0,     0,   141,     0,   143,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,     0,    33,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35
};

static const yytype_int16 yycheck[] =
{
       4,    28,     6,     4,    51,     6,    38,    39,    40,    41,
      42,    46,    44,    48,    51,    47,    44,    49,    51,    23,
      38,    39,    40,    41,    42,    29,    30,    50,    51,    30,
      31,    39,    40,    41,    42,    50,    51,    50,    51,    51,
      45,    38,    51,    43,     0,    51,   122,   117,    51,    51,
     115,    -1,    51,    51,    50,    50,    50,    37,    51,    50,
      50,    50,    50,    50,    50,    69,    51,    50,    69,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    50,    24,    50,    26,    50,
      50,   118,    50,    50,    -1,    50,    -1,    51,    51,    51,
      50,    50,    50,    -1,    51,    51,    51,    51,    51,    51,
      51,   115,    51,    -1,    51,    51,    51,    51,   122,    51,
      51,   122,    50,    50,    48,    48,    51,    -1,    -1,    51,
      68,    51,    51,    51,    51,   139,    -1,    -1,   139,    -1,
      -1,    -1,    -1,   147,   148,   149,   147,   148,   149,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      -1,    -1,   100,   101,   102,   103,   104,    -1,   106,   107,
     108,   109,   110,    -1,    -1,    -1,   114,    -1,   116,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    51
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    36,    37,    51,    54,    55,    51,    51,
      51,    38,    39,    40,    41,    42,    58,    59,    51,    58,
      59,    44,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    47,    49,
      58,    59,    61,    63,    61,    45,    62,    61,    56,    58,
      56,    58,    57,    58,    59,    59,    43,    60,    51,     0,
      54,    51,    51,    51,    51,    50,    50,    50,    51,    51,
      50,    50,    50,    50,    50,    51,    50,    50,    50,    50,
      50,    61,    58,    59,    50,    50,    50,    50,    50,    51,
      51,    51,    50,    51,    51,    51,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    46,
      48,    61,    63,    61,    62,    56,    57,    50,    50,    50,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      58,    59,    51,    51,    51,    51,    58,    59,    58,    59,
      58,    59,    48,    48,    51,    51,    51,    51,    51,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    53,    54,    54,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    56,    56,    57,    57,    57,    58,
      59,    59,    59,    59,    60,    61,    62,    63,    63,    63,
      63,    63,    63,    63,    63
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     3,     3,     3,     3,     3,     2,
       3,     1,     2,     2,     2,     2,     3,     3,     3,     3,
       7,     7,     7,     7,     7,     7,     3,     3,     5,     5,
       5,     5,     5,     3,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     3,     5,     5
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 4:
#line 109 "./auxiliary/parser.y"
                                       { std::cout << ".word" << std::endl; build_directive(type::DIRECTIVE_TYPE::WORD); }
#line 1518 "./auxiliary/src/parser.cpp"
    break;

  case 5:
#line 110 "./auxiliary/parser.y"
                                   { std::cout << ".global" << std::endl; build_directive(type::DIRECTIVE_TYPE::GLOBAL); }
#line 1524 "./auxiliary/src/parser.cpp"
    break;

  case 6:
#line 111 "./auxiliary/parser.y"
                                   { std::cout << ".extern" << std::endl; build_directive(type::DIRECTIVE_TYPE::EXTERN); }
#line 1530 "./auxiliary/src/parser.cpp"
    break;

  case 7:
#line 112 "./auxiliary/parser.y"
                                      { std::cout << ".section" << std::endl; build_directive(type::DIRECTIVE_TYPE::SECTION); }
#line 1536 "./auxiliary/src/parser.cpp"
    break;

  case 8:
#line 113 "./auxiliary/parser.y"
                                      { std::cout << ".skip" << std::endl; build_directive(type::DIRECTIVE_TYPE::SKIP); }
#line 1542 "./auxiliary/src/parser.cpp"
    break;

  case 9:
#line 114 "./auxiliary/parser.y"
                    { std::cout << ".end" << std::endl; build_directive(type::DIRECTIVE_TYPE::END); }
#line 1548 "./auxiliary/src/parser.cpp"
    break;

  case 10:
#line 115 "./auxiliary/parser.y"
                                     { std::cout << ".ascii" << std::endl; build_directive(type::DIRECTIVE_TYPE::ASCII); }
#line 1554 "./auxiliary/src/parser.cpp"
    break;

  case 11:
#line 116 "./auxiliary/parser.y"
        { std::cout << "labela: " << (yyvsp[0].symbol_name) << std::endl; Assembler::get_instance().add_symbol((yyvsp[0].symbol_name)); }
#line 1560 "./auxiliary/src/parser.cpp"
    break;

  case 12:
#line 117 "./auxiliary/parser.y"
                       { std::cout << "halt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::HALT); }
#line 1566 "./auxiliary/src/parser.cpp"
    break;

  case 13:
#line 118 "./auxiliary/parser.y"
                      { std::cout << "int" << std::endl; build_instruction(type::INSTRUCTION_TYPE::INT); }
#line 1572 "./auxiliary/src/parser.cpp"
    break;

  case 14:
#line 119 "./auxiliary/parser.y"
                       { std::cout << "iret" << std::endl; build_instruction(type::INSTRUCTION_TYPE::IRET); }
#line 1578 "./auxiliary/src/parser.cpp"
    break;

  case 15:
#line 120 "./auxiliary/parser.y"
                      { std::cout << "ret" << std::endl; build_instruction(type::INSTRUCTION_TYPE::RET); }
#line 1584 "./auxiliary/src/parser.cpp"
    break;

  case 16:
#line 121 "./auxiliary/parser.y"
                                     { std::cout << "call" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CALL); }
#line 1590 "./auxiliary/src/parser.cpp"
    break;

  case 17:
#line 122 "./auxiliary/parser.y"
                                        { std::cout << "call" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CALL); }
#line 1596 "./auxiliary/src/parser.cpp"
    break;

  case 18:
#line 123 "./auxiliary/parser.y"
                                    { std::cout << "jmp" << std::endl; build_instruction(type::INSTRUCTION_TYPE::JMP); }
#line 1602 "./auxiliary/src/parser.cpp"
    break;

  case 19:
#line 124 "./auxiliary/parser.y"
                                       { std::cout << "jmp" << std::endl; build_instruction(type::INSTRUCTION_TYPE::JMP); }
#line 1608 "./auxiliary/src/parser.cpp"
    break;

  case 20:
#line 125 "./auxiliary/parser.y"
                                                                            { std::cout << "beq" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BEQ); }
#line 1614 "./auxiliary/src/parser.cpp"
    break;

  case 21:
#line 126 "./auxiliary/parser.y"
                                                                               { std::cout << "beq" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BEQ); }
#line 1620 "./auxiliary/src/parser.cpp"
    break;

  case 22:
#line 127 "./auxiliary/parser.y"
                                                                            { std::cout << "bne" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BNE); }
#line 1626 "./auxiliary/src/parser.cpp"
    break;

  case 23:
#line 128 "./auxiliary/parser.y"
                                                                               { std::cout << "bne" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BNE); }
#line 1632 "./auxiliary/src/parser.cpp"
    break;

  case 24:
#line 129 "./auxiliary/parser.y"
                                                                            { std::cout << "bgt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BGT); }
#line 1638 "./auxiliary/src/parser.cpp"
    break;

  case 25:
#line 130 "./auxiliary/parser.y"
                                                                               { std::cout << "bgt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BGT); }
#line 1644 "./auxiliary/src/parser.cpp"
    break;

  case 26:
#line 131 "./auxiliary/parser.y"
                                     { std::cout << "push" << std::endl; build_instruction(type::INSTRUCTION_TYPE::PUSH); }
#line 1650 "./auxiliary/src/parser.cpp"
    break;

  case 27:
#line 132 "./auxiliary/parser.y"
                                    { std::cout << "pop" << std::endl; build_instruction(type::INSTRUCTION_TYPE::POP); }
#line 1656 "./auxiliary/src/parser.cpp"
    break;

  case 28:
#line 133 "./auxiliary/parser.y"
                                                         { std::cout << "xchg" << std::endl; build_instruction(type::INSTRUCTION_TYPE::XCHG); }
#line 1662 "./auxiliary/src/parser.cpp"
    break;

  case 29:
#line 134 "./auxiliary/parser.y"
                                                        { std::cout << "add" << std::endl; build_instruction(type::INSTRUCTION_TYPE::ADD); }
#line 1668 "./auxiliary/src/parser.cpp"
    break;

  case 30:
#line 135 "./auxiliary/parser.y"
                                                        { std::cout << "sub" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SUB); }
#line 1674 "./auxiliary/src/parser.cpp"
    break;

  case 31:
#line 136 "./auxiliary/parser.y"
                                                        { std::cout << "mul" << std::endl; build_instruction(type::INSTRUCTION_TYPE::MUL); }
#line 1680 "./auxiliary/src/parser.cpp"
    break;

  case 32:
#line 137 "./auxiliary/parser.y"
                                                        { std::cout << "div" << std::endl; build_instruction(type::INSTRUCTION_TYPE::DIV); }
#line 1686 "./auxiliary/src/parser.cpp"
    break;

  case 33:
#line 138 "./auxiliary/parser.y"
                                    { std::cout << "not" << std::endl; build_instruction(type::INSTRUCTION_TYPE::NOT); }
#line 1692 "./auxiliary/src/parser.cpp"
    break;

  case 34:
#line 139 "./auxiliary/parser.y"
                                                        { std::cout << "and" << std::endl; build_instruction(type::INSTRUCTION_TYPE::AND); }
#line 1698 "./auxiliary/src/parser.cpp"
    break;

  case 35:
#line 140 "./auxiliary/parser.y"
                                                       { std::cout << "or" << std::endl; build_instruction(type::INSTRUCTION_TYPE::OR); }
#line 1704 "./auxiliary/src/parser.cpp"
    break;

  case 36:
#line 141 "./auxiliary/parser.y"
                                                        { std::cout << "xor" << std::endl; build_instruction(type::INSTRUCTION_TYPE::XOR); }
#line 1710 "./auxiliary/src/parser.cpp"
    break;

  case 37:
#line 142 "./auxiliary/parser.y"
                                                        { std::cout << "shl" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SHL); }
#line 1716 "./auxiliary/src/parser.cpp"
    break;

  case 38:
#line 143 "./auxiliary/parser.y"
                                                        { std::cout << "shr" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SHR); }
#line 1722 "./auxiliary/src/parser.cpp"
    break;

  case 39:
#line 144 "./auxiliary/parser.y"
                                                          { std::cout << "csrrd" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CSRRD); }
#line 1728 "./auxiliary/src/parser.cpp"
    break;

  case 40:
#line 145 "./auxiliary/parser.y"
                                                          { std::cout << "csrwr" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CSRWR); }
#line 1734 "./auxiliary/src/parser.cpp"
    break;

  case 41:
#line 146 "./auxiliary/parser.y"
                                                       { std::cout << "ld" << std::endl; build_instruction(type::INSTRUCTION_TYPE::LD); }
#line 1740 "./auxiliary/src/parser.cpp"
    break;

  case 42:
#line 147 "./auxiliary/parser.y"
                                                       { std::cout << "st" << std::endl; build_instruction(type::INSTRUCTION_TYPE::ST); }
#line 1746 "./auxiliary/src/parser.cpp"
    break;

  case 49:
#line 163 "./auxiliary/parser.y"
       { std::cout << "simbol: " << (yyvsp[0].symbol_name) << std::endl;  CommandBuilder::get_instance().enque_param(Assembler::get_instance().create_symbol((yyvsp[0].symbol_name))); }
#line 1752 "./auxiliary/src/parser.cpp"
    break;

  case 50:
#line 167 "./auxiliary/parser.y"
            { std::cout << "literal: " << std::stoi((yyvsp[0].literal_value) + 2, 0, 2) << std::endl; CommandBuilder::get_instance().enque_param(new Literal(std::stoi((yyvsp[0].literal_value) + 2, 0, 2))); }
#line 1758 "./auxiliary/src/parser.cpp"
    break;

  case 51:
#line 168 "./auxiliary/parser.y"
              { std::cout << "literal: " << std::stoi((yyvsp[0].literal_value), 0, 8) << std::endl;   CommandBuilder::get_instance().enque_param(new Literal(std::stoi((yyvsp[0].literal_value), 0, 8))); }
#line 1764 "./auxiliary/src/parser.cpp"
    break;

  case 52:
#line 169 "./auxiliary/parser.y"
              { std::cout << "literal: " << std::stoi((yyvsp[0].literal_value), 0, 10) << std::endl;  CommandBuilder::get_instance().enque_param(new Literal(std::stoi((yyvsp[0].literal_value), 0, 10))); }
#line 1770 "./auxiliary/src/parser.cpp"
    break;

  case 53:
#line 170 "./auxiliary/parser.y"
              { std::cout << "literal: " << std::stoi((yyvsp[0].literal_value), 0, 16) << std::endl;  CommandBuilder::get_instance().enque_param(new Literal(std::stoi((yyvsp[0].literal_value), 0, 16))); }
#line 1776 "./auxiliary/src/parser.cpp"
    break;

  case 54:
#line 174 "./auxiliary/parser.y"
               { std::cout << "literal: " << (yyvsp[0].literal_value) << std::endl; CommandBuilder::get_instance().enque_param(new Literal((yyvsp[0].literal_value))); }
#line 1782 "./auxiliary/src/parser.cpp"
    break;

  case 55:
#line 178 "./auxiliary/parser.y"
       { std::cout << "GP registar: " << (yyvsp[0].gpr_index) << std::endl; CommandBuilder::get_instance().set_next_gp_reg(static_cast<type::GP_REG>((yyvsp[0].gpr_index))); }
#line 1788 "./auxiliary/src/parser.cpp"
    break;

  case 56:
#line 182 "./auxiliary/parser.y"
       { std::cout << "CS registar: " << (yyvsp[0].csr_index) << std::endl; CommandBuilder::get_instance().set_next_cs_reg(static_cast<type::CS_REG>((yyvsp[0].csr_index))); }
#line 1794 "./auxiliary/src/parser.cpp"
    break;

  case 57:
#line 186 "./auxiliary/parser.y"
                        { std::cout << "immed" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::IMMED); }
#line 1800 "./auxiliary/src/parser.cpp"
    break;

  case 58:
#line 187 "./auxiliary/parser.y"
                       { std::cout << "immed" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::IMMED); }
#line 1806 "./auxiliary/src/parser.cpp"
    break;

  case 59:
#line 188 "./auxiliary/parser.y"
                   { std::cout << "mem dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::MEM_DIR); }
#line 1812 "./auxiliary/src/parser.cpp"
    break;

  case 60:
#line 189 "./auxiliary/parser.y"
                 { std::cout << "mem dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::MEM_DIR); }
#line 1818 "./auxiliary/src/parser.cpp"
    break;

  case 61:
#line 190 "./auxiliary/parser.y"
                 { std::cout << "reg dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_DIR); }
#line 1824 "./auxiliary/src/parser.cpp"
    break;

  case 62:
#line 191 "./auxiliary/parser.y"
                                                   { std::cout << "reg mem" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND); }
#line 1830 "./auxiliary/src/parser.cpp"
    break;

  case 63:
#line 192 "./auxiliary/parser.y"
                                                                        { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }
#line 1836 "./auxiliary/src/parser.cpp"
    break;

  case 64:
#line 193 "./auxiliary/parser.y"
                                                                     { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }
#line 1842 "./auxiliary/src/parser.cpp"
    break;


#line 1846 "./auxiliary/src/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 195 "./auxiliary/parser.y"
