%{
  #include "parser.tab.h"
%}

%option noyywrap

%%

"ld" { yylval.instruction_alias = "ld"; return INSTRUCTION; }
"jmp" { yylval.instruction_alias = "jmp"; return INSTRUCTION; }
"je" { yylval.instruction_alias = "je"; return INSTRUCTION; }
".section" { yylval.instruction_alias = ".section"; return DIRECTIVE; }
\n { return EOL; }
. { }

%%