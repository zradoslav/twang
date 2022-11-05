#ifndef FORM_HPP
#define FORM_HPP

#include <string>

namespace twang::forms {

class form {
public:
	enum type {
		SYMBOL,
		STRING,
		NUMBER,
		CHAR,
		NIL,
		BOOLEAN,
		SYMVAL,
		KEYWORD,
		LIST,
		VECTOR,
		MAP,
		SET 
	};

	form(type t);
	virtual ~form() = default;

	type form_type() const;

	virtual form* eval() = 0;
	virtual std::string print() const = 0;

private:
	type m_type;
};

}

#endif
