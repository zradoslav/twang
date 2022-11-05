#ifndef FORM_BOOLEAN_HPP
#define FORM_BOOLEAN_HPP

#include "form.hpp"

namespace twang::forms {

class boolean : public form {
public:
	boolean(bool value);
	~boolean() = default;

	form* eval() override;
	std::string print() const override;

private:
	bool m_data;
};

}

#endif
