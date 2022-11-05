%skeleton "lalr1.cc"
%require  "3.0"
%defines 
%define api.namespace {twang}
%define api.parser.class {parser}

%code requires {
   #include <stack>
   #include "location.hh"

   namespace twang {
      class scanner;
      class reader;
   }

// The following definitions is missing when "%locations" isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "forms/all.hpp"

static std::stack<twang::forms::form*> _stack;

}

%parse-param { scanner& __scanner }
%parse-param { reader& __reader }

%code{
   #include <iostream>

   #include "reader.h"
   #include "scanner.h"
 
   #undef yylex
   #define yylex __scanner.yylex
}

%define api.value.type variant
%define parse.assert
%define parse.error verbose

%token               END    0     "end of file"
%token               BADTOKEN

%token               LPAREN RPAREN
%token               LBRACE RBRACE
%token               LBRACK RBRACK

%token               HASH SLASH DOT COLON

%token <std::string> IDENTIFIER

%token <std::string> STRING
%token <bool>        BOOLEAN
%token <nullptr_t>   NIL
/* Numbers */
%token <long long>   INTEGER
%token <double>      FLOAT
%token <std::tuple<long long, unsigned long long>> RATIO

%type <twang::forms::form*> form symbol literal list map set vector number
%type <std::string> keyword
%type <std::pair<twang::forms::form*, twang::forms::form*>> form-pair

%locations

%%

program   : {_stack.push(new twang::forms::vector()); }
            forms {
                std::cout << "PARSED AST(s):\n\n";
                auto t = _stack.top();
                std::cout << t->print() << std::endl;
            }
          ;

form      : symbol { $$ = $1; }
          | literal { $$ = $1; }
          | list { $$ = $1; }
          | vector { $$ = $1; }
          | map { $$ = $1; }
          | set { $$ = $1; }
          ;
forms     : %empty
          | forms form {
              auto t = _stack.top();
              switch(t->form_type()) {
                case twang::forms::form::LIST: {
                    auto l = dynamic_cast<twang::forms::list*>(t);
                    l->append($2);
                    break;
                }
                case twang::forms::form::VECTOR: {
                    auto v = dynamic_cast<twang::forms::vector*>(t);
                    v->append($2);
                    break;
                }
                case twang::forms::form::SET: {
                    auto s = dynamic_cast<twang::forms::set*>(t);
                    s->add($2);
                    break;
                }
                default:
                    std::cout << "ill type: " << t->form_type() << "\n";
                    assert(0);
              }
          }
          ;

form-pair : form form { $$ = std::make_pair($1, $2); }
form-pairs: %empty
          | form-pairs form-pair {
              auto t = _stack.top();
              assert(t->form_type() == twang::forms::form::MAP);
              dynamic_cast<twang::forms::map*>(t)->add($2.first, $2.second);
          }
          ;

list      : LPAREN { _stack.push(new twang::forms::list()); }
            forms
            RPAREN {
                $$ = _stack.top();
                _stack.pop();
            }
            ;

vector    : LBRACK { _stack.push(new twang::forms::vector()); }
            forms
            RBRACK {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

map       : LBRACE { _stack.push(new twang::forms::map()); }
            form-pairs
            RBRACE {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

set       : HASH LBRACE { _stack.push(new twang::forms::set()); }
            forms
            RBRACE {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

symbol    : IDENTIFIER { $$ = new twang::forms::symbol($1); }
          | IDENTIFIER COLON { $$ = new twang::forms::symbol($1 + ":"); }
          | symbol SLASH symbol {
                auto s1 = dynamic_cast<twang::forms::symbol*>($1)->name();
                auto s3 = dynamic_cast<twang::forms::symbol*>($3)->name();
                delete $1;
                delete $3;
                $$ = new twang::forms::symbol(s1 + "/" + s3);
            }
          | symbol DOT symbol {
                auto s1 = dynamic_cast<twang::forms::symbol*>($1)->name();
                auto s3 = dynamic_cast<twang::forms::symbol*>($3)->name();
                delete $1;
                delete $3;
                $$ = new twang::forms::symbol(s1 + "." + s3);
            }
          ;

literal   : STRING  { $$ = new twang::forms::string($1); }
          | number  { $$ = $1; }
          // | character
          | NIL     { $$ = new twang::forms::nil(); }
          | BOOLEAN { $$ = new twang::forms::boolean($1); }
          // | symval
          | keyword { $$ = new twang::forms::keyword($1); }
          ;

number    : INTEGER { $$ = new twang::forms::integer($1); }
          | FLOAT   { $$ = new twang::forms::floating($1); }
          | RATIO   { $$ = new twang::forms::ratio($1); }
          ;

keyword   : COLON IDENTIFIER { $$ = ":" + $2; }

%%

void 
twang::parser::error(const twang::parser::location_type &l, const std::string &err_message) {
    std::cout << "Parsing Error: " << err_message << " at " << l << "\n";
}
