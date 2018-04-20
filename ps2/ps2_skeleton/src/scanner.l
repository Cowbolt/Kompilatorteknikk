%{
#include "../include/vslc.h"
%}
%option noyywrap
%option array
%option yylineno
comment \/\/[^\n]*\n
digit [0-9]
%%

FUNC                   	{return FUNC; }
BEGIN					{return OPENBLOCK; }
END						{return CLOSEBLOCK; }
RETURN					{return RETURN; }
PRINT					{return PRINT; }
CONTINUE				{return CONTINUE; }
IF						{return IF; }
THEN					{return THEN; }
ELSE					{return ELSE; }
WHILE					{return WHILE; }
DO						{return DO; }
VAR						{return VAR; }
">>"					{return RSHIFT; }
"<<"					{return LSHIFT; }
[\t\n\r\v\ ]+			;
{digit}+				{return NUMBER; }
{comment}+				;
\"[^\n]*\"				{return STRING; }
[a-zA-Z_][a-zA-Z0-9_]*	{return IDENTIFIER; }
.                      	{ return yytext[0]; }
%%
