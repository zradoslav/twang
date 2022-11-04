#ifndef TWANG_SCANNER_H
#define TWANG_SCANNER_H

/* From Flex documentation:
 *   The c++ scanner is a mess. The FlexLexer.h header file relies on the
 *   following macro. This is required in order to pass the c++-multiple-scanners
 *   test in the regression suite. We get reports that it breaks inheritance.
 *   We will address this in a future release of flex, or omit the C++ scanner
 *   altogether.
 *
 * So, for now, let's define this macro too...
 */
 
#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#undef yylex

#include "twang.tab.hh"

namespace twang {

class scanner : public yyFlexLexer {
public:
   
   scanner(std::istream *in) : yyFlexLexer(in) {
      loc = new parser::location_type();
   };
   virtual ~scanner() {
      delete loc;
   };

   // Get rid of override virtual function warning.
   using FlexLexer::yylex;

   virtual
   int yylex(parser::semantic_type * const lval, 
             parser::location_type *location);

private:
   parser::semantic_type *yylval = nullptr;
   parser::location_type *loc    = nullptr;
};

} // end namespace

#endif

