#ifndef FORM_KEYWORD_HPP
#define FORM_KEYWORD_HPP

#include "form.hpp"

namespace twang::forms {

class keyword : public form {
public:
	keyword(const std::string& name);
	~keyword() = default;

	form* eval() override;
	std::string print() const override;

	std::string name() const;

private:
	std::string m_name;
};

}

#endif
