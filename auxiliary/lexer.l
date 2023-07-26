%{
  #include "../inc/parser.hpp"
%}

%option noyywrap

%%

".global" { return DIRECTIVE_GLOBAL; }
".word" { return DIRECTIVE_WORD; }

[._A-Za-z][._0-9A-Za-z]* { yylval.symbol_name = yytext; return SYMBOL; }

0b[0-1]+ { yylval.literal_value = yytext; return LITERAL_BIN; }
0[0-7]* { yylval.literal_value = yytext; return LITERAL_OCT; }
[1-9][0-9]* { yylval.literal_value = yytext; return LITERAL_DEC; }
0x[0-9a-fA-F]+ { yylval.literal_value = yytext; return LITERAL_HEX; }


"," { return COMMA; }
\n { return EOL; }
[ \t]+ { }
. { return UNDEFIEND; }

%%