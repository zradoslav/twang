#ifndef FORM_VECTOR_HPP
#define FORM_VECTOR_HPP

#include "form.hpp"

#include <vector>

namespace twang::forms {

class vector : public form {
public:
	vector();
	~vector() = default;

	form* eval() override;
	std::string print() const override;

	void append(form* f);

private:
	std::vector<form*> m_data;
};

}

#endif
