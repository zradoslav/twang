#ifndef FORM_MAP_HPP
#define FORM_MAP_HPP

#include "form.hpp"

#include <map>

namespace twang::forms {

class map : public form {
public:
	map();
	~map() = default;

	form* eval() override;
	std::string print() const override;

	void add(form* key, form* value);

private:
	std::map<form*, form*> m_data;
};

}

#endif
