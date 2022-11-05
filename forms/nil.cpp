#include "nil.hpp"

namespace twang::forms {

nil::nil()
	: form(NIL)
{}

form* nil::eval() {
	return this;
}

std::string nil::print() const {
	return "nil";
}

}
