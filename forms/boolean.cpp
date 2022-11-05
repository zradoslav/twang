#include "boolean.hpp"

namespace twang::forms {

boolean::boolean(bool value)
	: form(BOOLEAN)
	, m_data(value)
{}

form* boolean::eval() {
	return this;
}

std::string boolean::print() const {
	return m_data ? "true" : "false";
}

}
