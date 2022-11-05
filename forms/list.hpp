#ifndef FORM_LIST_HPP
#define FORM_LIST_HPP

#include "form.hpp"

#include <list>

namespace twang::forms {

class list : public form {
public:
	list();
	~list() = default;

	form* eval() override;
	std::string print() const override;

	void append(form* f);

private:
	std::list<form*> m_data;
};

}

#endif
