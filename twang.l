%{
#include <string>

#include "form.h"

/* Implementation of yyFlexScanner */
#include "scanner.h"
#undef  YY_DECL
#define YY_DECL int twang::scanner::yylex( \
  twang::parser::semantic_type * const lval, \
  twang::parser::location_type * loc)

/* typedef to make the returns for the tokens shorter */
using token = twang::parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

int to_int_base(const std::string& s, int base);

%}

%option nodefault nounistd nounput
%option noyywrap
%option yyclass="twang::scanner"
%option c++

%x COMMENT

/* core rules */

alpha     [A-Za-z]
digit     [0-9]
octdig    [0-7]
hexdig    {digit}|[A-Fa-f]
dquote    \"
vchar     [\x21-\x7E]
nonnum    {alpha}|"*"|"+"|"!"|"-"|"_"|"'"|"?"|"<"|">"|"="
allowed   {nonnum}|{digit}

%%
%{
/* Executed at the beginning of yylex */
yylval = lval;
%}

[ ,\t]   {}
\n      { loc->lines(); }

 /* Comment */

;             { BEGIN(COMMENT); }
<COMMENT>.    { yymore(); }
<COMMENT>\n   { loc->lines(); BEGIN(0); }

 /* Syntax */

"(" { return token::LPAREN; }
")" { return token::RPAREN; }
"[" { return token::LBRACK; }
"]" { return token::RBRACK; }
"{" { return token::LBRACE; }
"}" { return token::RBRACE; }

"#" { return token::HASH; }
"/" { return token::SLASH; }
"\." { return token::DOT; }
"\:" { return token::COLON; }


 /** Literals **/

 /* Strings */
{dquote}{vchar}*{dquote} {
    yylval->emplace<std::string>(yytext + 1, yyleng - 2);
    return token::STRING;
}

 /* Numbers */
[+-]?[1-9]{digit}*N? {
    int len = yyleng - 1;
    if (yytext[len] == 'N') yytext[len] = '\0';
    yylval->build<int>(to_int_base(yytext, 10));
    return token::INTEGER;
}

[+-]?0{octdig}*N? {
    int len = yyleng - 1;
    if (yytext[len] == 'N') yytext[len] = '\0';
    yylval->build<int>(to_int_base(yytext, 8));
    return token::INTEGER;
}

[+-]?0[Xx]{hexdig}+N? {
    int len = yyleng - 1;
    if (yytext[len] == 'N') yytext[len] = '\0';
    yylval->build<int>(to_int_base(yytext, 16));
    return token::INTEGER;
}

[+-]?{digit}{1,2}r({digit}|{alpha})+ {
    int pos = 0;
    std::string v(yytext, yyleng);
    if (v[0] == '+' || v[0] == '-')
        pos = 1;

    auto delim = v.find("r");
    int base = to_int_base(v.substr(pos, delim), 10);
    if (base < 2 || base > 32)
        return token::BADTOKEN;

    int value = to_int_base(v.substr(delim + 1), base);
    yylval->build<int>(value);
    return token::INTEGER;
}

 /* TODO: float */

[+-]?{digit}+"/"{digit}+ {
    std::string v(yytext, yyleng);
    auto delim = v.find("/");
    int numerator = to_int_base(v.substr(0, delim), 10);
    int denominator = to_int_base(v.substr(delim + 1), 10);

    yylval->emplace<number::ratio>(numerator, denominator);
    return token::RATIO;
}

 /* Characters */



 /* Constants */

nil   { return token::NIL; }
true  { yylval->build<bool>(true); return token::BOOLEAN; }
false { yylval->build<bool>(false); return token::BOOLEAN; }

 /** Symbols **/

{nonnum}{allowed}* {
    yylval->build<std::string>(yytext);
    return token::IDENTIFIER;
}

 /* Unmatched */

. { return yytext[0]; }

%%


int to_int_base(const std::string& s, int base) {
    return std::stoi(s, nullptr, base);
}
