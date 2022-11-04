%skeleton "lalr1.cc"
%require  "3.0"
%defines 
%define api.namespace {twang}
%define api.parser.class {parser}

%code requires{
   #include <iomanip>
   #include <stack>
   #include "form.h"
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

static std::stack<form*> _stack;

static void print(form* f);
static void print_list(list* l);
static void print_vector(vector* v);
static void print_map(map* m);
static void print_set(set* s);

void print(form* f) {
    if (!f) return;
    std::string as_string;
    switch(f->type()) {
        case SYMBOL:
            as_string = dynamic_cast<symbol*>(f)->name();
            break;
        case STRING:
            as_string = dynamic_cast<string*>(f)->value();
            break;
        case NIL:
            as_string = "nil";
            break;
        case BOOLEAN:
            as_string = dynamic_cast<boolean*>(f)->value() ? "true" : "false";
            break;
        case KEYWORD:
            as_string = dynamic_cast<keyword*>(f)->name();
            break;
        case LIST:
            print_list(dynamic_cast<list*>(f));
            break;
        case VECTOR:
            print_vector(dynamic_cast<vector*>(f));
            break;
        case MAP:
            print_map(dynamic_cast<map*>(f));
            break;
        case SET:
            print_set(dynamic_cast<set*>(f));
            break;
    }
    std::cout << as_string;
}

void print_list(list* l) {
    std::cout << "(";

    auto c = l->collection();
    if (!c.empty()) {
        auto last = c.end();
        last--;
        for (auto it = c.begin(); it != last; it++) {
            print(*it);
            std::cout << " ";
        }
        print(c.back());
    }

    std::cout << ")";
}

void print_vector(vector* v) {
    std::cout << "[";

    auto c = v->collection();
    if (!c.empty()) {
        auto last = c.end();
        last--;
        for (auto it = c.begin(); it != last; it++) {
            print(*it);
            std::cout << " ";
        }
        print(c.back());
    }

    std::cout << "]";
}

void print_map(map* m) {
    std::cout << "{";

    auto c = m->collection();
    if (!c.empty()) {
        auto last = c.end();
        last--;
        for (auto it = c.begin(); it != last; it++) {
            print(it->first);
            std::cout << " ";
            print(it->second);
            std::cout << ", ";
        }
        print(last->first);
        std::cout << " ";
        print(last->second);
    }

    std::cout << "}";
}

void print_set(set* s) {
    std::cout << "#{";

    auto c = s->collection();
    if (!c.empty()) {
        auto last = c.end();
        last--;
        for (auto it = c.begin(); it != last; it++) {
            print(*it);
            std::cout << " ";
        }
        print(*last);
    }

    std::cout << "}";
}

}

%parse-param { scanner& __scanner }
%parse-param { reader& __reader }

%code{
   #include <iostream>
   
   //#include "form.h"

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
%token <int>           INTEGER
%token <double>        FLOAT
%token <number::ratio> RATIO

%type <form*> form symbol literal list map set vector number
%type <std::string> keyword
%type <std::pair<form*, form*>> form-pair

%locations

%%

program   : {_stack.push(new vector()); }
            forms {
                std::cout << "PARSED AST(s):\n\n";
                auto t = dynamic_cast<vector*>(_stack.top());
                for (auto node : t->collection()) {
                    print(node);
                    std::cout << "\n";
                }
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
              switch(t->type()) {
                case LIST: {
                    auto l = dynamic_cast<list*>(t);
                    l->append($2);
                    break;
                }
                case VECTOR: {
                    auto v = dynamic_cast<vector*>(t);
                    v->append($2);
                    break;
                }
                case SET: {
                    auto s = dynamic_cast<set*>(t);
                    s->add($2);
                    break;
                }
                default:
                    std::cout << "ill type: " << t->type() << "\n";
                    assert(0);
              }
          }
          ;

form-pair : form form { $$ = std::make_pair($1, $2); }
form-pairs: %empty
          | form-pairs form-pair {
              auto t = _stack.top();
              assert(t->type() == MAP);
              dynamic_cast<map*>(t)->add($2.first, $2.second);
          }
          ;

list      : LPAREN { _stack.push(new list()); }
            forms
            RPAREN {
                $$ = _stack.top();
                _stack.pop();
            }
            ;

vector    : LBRACK { _stack.push(new vector()); }
            forms
            RBRACK {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

map       : LBRACE { _stack.push(new map()); }
            form-pairs
            RBRACE {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

set       : HASH LBRACE { _stack.push(new set()); }
            forms
            RBRACE {
                $$ = _stack.top();
                _stack.pop();
            }
          ;

symbol    : IDENTIFIER { $$ = new symbol($1); }
          | IDENTIFIER COLON { $$ = new symbol($1 + ":"); }
          | symbol SLASH symbol {
                auto s1 = dynamic_cast<symbol*>($1)->name();
                auto s3 = dynamic_cast<symbol*>($3)->name();
                delete $1;
                delete $3;
                $$ = new symbol(s1 + "/" + s3);
            }
          | symbol DOT symbol {
                auto s1 = dynamic_cast<symbol*>($1)->name();
                auto s3 = dynamic_cast<symbol*>($3)->name();
                delete $1;
                delete $3;
                $$ = new symbol(s1 + "." + s3);
            }
          ;

literal   : STRING  { $$ = new string($1); }
          | number  { $$ = $1; }
          // | character
          | NIL     { $$ = new nil(); }
          | BOOLEAN { $$ = new boolean($1); }
          // | symval
          | keyword { $$ = new keyword($1); }
          ;

number    : INTEGER { $$ = new number($1); }
          | FLOAT   { $$ = new number($1); }
          | RATIO   { /*$$ = new number(std::get<0>($1), std::get<1>($1));*/ }
          ;

keyword   : COLON IDENTIFIER { $$ = ":" + $2; }

%%

void 
twang::parser::error(const twang::parser::location_type &l, const std::string &err_message) {
   std::cout << "Parsing Error: " << err_message << " at " << l << "\n";
}
