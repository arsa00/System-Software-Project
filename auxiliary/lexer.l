%{
  #include "../inc/parser.hpp"
  #include <string>
  #include <iostream>
%}

%option noyywrap

%%

".global" { return DIRECTIVE_GLOBAL; }
".word" { return DIRECTIVE_WORD; }

[._A-Za-z][._0-9A-Za-z]* { yylval.symbol_name = yytext; return SYMBOL; }

0b[0-1]+ { yylval.literal_value = std::stoi(yytext+2, 0, 2); return LITERAL; }
0[0-7]* { yylval.literal_value = std::stoi(yytext, 0, 8); return LITERAL; }
[1-9][0-9]* { yylval.literal_value = std::stoi(yytext, 0, 10); return LITERAL; }
0x[0-9a-fA-F]+ { yylval.literal_value = std::stoi(yytext, 0, 16); return LITERAL; }


"," { return COMMA; }
\n { return EOL; }
[ \t]+ { }
. { return UNDEFIEND; }

%%