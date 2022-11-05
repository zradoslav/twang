#include "vector.hpp"

#include <sstream>

namespace twang::forms {

vector::vector()
	: form(VECTOR)
	, m_data()
{}

form* vector::eval() {
	// todo
#warning "not implemented"
}

std::string vector::print() const {
	std::ostringstream oss;
	oss << "[";

	auto c = m_data;
	if (!c.empty()) {
		auto last = c.end();
		last--;
		for (auto it = c.begin(); it != last; it++) {
			oss << (*it)->print() << " ";
		}
		oss << (*last)->print();
	}
	oss<< "]";

	return oss.str();
}

void vector::append(form* f) {
	m_data.push_back(f);
}

}
