#ifndef FORM_STRING_HPP
#define FORM_STRING_HPP

#include "form.hpp"

namespace twang::forms {

class string : public form {
public:
	string(const std::string& value);
	~string() = default;

	form* eval() override;
	std::string print() const override;

private:
	std::string m_data;
};

}

#endif
