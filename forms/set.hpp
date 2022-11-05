#ifndef FORM_SET_HPP
#define FORM_SET_HPP

#include "form.hpp"

#include <set>

namespace twang::forms {

class set : public form {
public:
	set();
	~set() = default;

	form* eval() override;
	std::string print() const override;

	void add(form* f);

private:
	std::set<form*> m_data;
};

}

#endif
