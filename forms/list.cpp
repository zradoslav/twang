#include "list.hpp"

#include <sstream>

namespace twang::forms {

list::list()
	: form(LIST)
	, m_data()
{}

form* list::eval() {
	// todo
#warning "not implemented"
}

std::string list::print() const {
	std::ostringstream oss;

	oss << "(";
	auto c = m_data;
	if (!c.empty()) {
		auto last = c.end();
		last--;
		for (auto it = c.begin(); it != last; it++) {
			oss << (*it)->print() << " ";
		}
		oss << (*last)->print();
	}
	oss << ")";

	return oss.str();
}

void list::append(form* f) {
	m_data.push_back(f);
}

}
