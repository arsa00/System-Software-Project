%{
  #include "../inc/parser.hpp"
  #include <string.h>
%}

%option noyywrap

%%

".global" { return DIRECTIVE_GLOBAL; }
".extern" { return DIRECTIVE_EXTERN; }
".section" { return DIRECTIVE_SECTION; }
".word" { return DIRECTIVE_WORD; }
".skip" { return DIRECTIVE_SKIP; }
".end" { return DIRECTIVE_END; }

[._A-Za-z][._0-9A-Za-z]* { yylval.symbol_name = yytext; return SYMBOL; }
[._A-Za-z][._0-9A-Za-z]*: { yytext[strlen(yytext)-1] = '\0'; yylval.symbol_name = yytext; return LABEL; }

0b[0-1]+ { yylval.literal_value = yytext; return LITERAL_BIN; }
0[0-7]* { yylval.literal_value = yytext; return LITERAL_OCT; }
[1-9][0-9]* { yylval.literal_value = yytext; return LITERAL_DEC; }
0x[0-9a-fA-F]+ { yylval.literal_value = yytext; return LITERAL_HEX; }


"," { return COMMA; }
\n { return EOL; }
#.*\n { return EOL; }
[ \t]+ { }
. { return UNDEFIEND; }

%%