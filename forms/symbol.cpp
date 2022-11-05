#include "symbol.hpp"

namespace twang::forms {

symbol::symbol(const std::string& name)
	: form(SYMBOL)
	, m_name(name)
{}

form* symbol::eval() {
	// TODO: resolve from env
	return this;
}

std::string symbol::print() const {
	return m_name;
}

std::string symbol::name() const {
	return m_name;
}

}
