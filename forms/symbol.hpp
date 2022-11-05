#ifndef FORM_SYMBOL_HPP
#define FORM_SYMBOL_HPP

#include "form.hpp"

namespace twang::forms {

class symbol : public form {
public:
	symbol(const std::string& name);
	~symbol() = default;

	form* eval() override;
	std::string print() const override;

	std::string name() const;

private:
	std::string m_name;
};

}

#endif
