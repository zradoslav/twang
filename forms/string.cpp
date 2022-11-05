#include "string.hpp"

namespace twang::forms {

string::string(const std::string& value)
	: form(STRING)
	, m_data(value)
{}

form* string::eval() {
	return this;
}

std::string string::print() const {
	return m_data;
}

}
