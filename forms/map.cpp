#include "map.hpp"

#include <sstream>

namespace twang::forms {

map::map()
	: form(MAP)
	, m_data()
{}

form* map::eval() {
	// todo
#warning "not implemented"
}

std::string map::print() const {
	std::ostringstream oss;
	oss << "{";

	auto c = m_data;
	if (!c.empty()) {
		auto last = c.end();
		last--;
		for (auto it = c.begin(); it != last; it++) {
			it->first->print();
			oss << " ";
			it->second->print();
			oss << ", ";
		}
		last->first->print();
		oss << " ";
		last->second->print();
	}
	oss << "}";

	return oss.str();
}

void map::add(form* key, form* value) {
	m_data[key] = value;
}

}
