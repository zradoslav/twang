#include "set.hpp"

#include <sstream>

namespace twang::forms {

set::set()
	: form(SET)
	, m_data()
{}

form* set::eval() {
	// todo
}

std::string set::print() const {
	std::ostringstream oss;

	oss << "#{";
	auto c = m_data;
	if (!c.empty()) {
		auto last = c.end();
		last--;
		for (auto it = c.begin(); it != last; it++) {
			oss << (*it)->print() << " ";
		}
		oss << (*last)->print();
	}
	oss << "}";

	return oss.str();
}

void set::add(form* f) {
	m_data.insert(f);
}

}
