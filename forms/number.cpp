#include "number.hpp"

#include <sstream>

namespace twang::forms {

number::number(type t)
	: form(NUMBER)
	, m_type(t)
{}

number::type number::number_type() const {
	return m_type;
}

/* number::integer */

integer::integer(long long value)
	: number(number::INTEGER)
	, m_data(value)
{}

form* integer::eval() {
	return this;
}

std::string integer::print() const {
	return std::to_string(m_data);
}

/* number::float */

floating::floating(double value)
	: number(number::DOUBLE)
	, m_data(value)
{}

form* floating::eval() {
	return this;
}

std::string floating::print() const {
	return std::to_string(m_data);
}

/* number::ratio */

ratio::ratio(long long n, unsigned long long d)
	: number(number::RATIO)
	, m_data(std::make_tuple(n, d)) 
{}

ratio::ratio(const std::tuple<long long, unsigned long long>& r)
	: number(number::RATIO)
	, m_data(r)
{}

form* ratio::eval() {
	return this;
}

std::string ratio::print() const {
	std::ostringstream oss;
	oss << std::get<0>(m_data) << "/" << std::get<1>(m_data);
	return oss.str();
}

}
