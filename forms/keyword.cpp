#include "keyword.hpp"

namespace twang::forms {

keyword::keyword(const std::string& name)
	: form(KEYWORD)
	, m_name(name)
{}

form* keyword::eval() {
	return this;
}

std::string keyword::print() const {
	return m_name;
}

std::string keyword::name() const {
	return m_name;
}

}
