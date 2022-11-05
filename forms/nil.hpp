#ifndef FORM_NIL_HPP
#define FORM_NIL_HPP

#include "form.hpp"

namespace twang::forms {

class nil : public form {
public:
	nil();
	~nil() = default;

	form* eval() override;
	std::string print() const override;
};

}

#endif
