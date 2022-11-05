#ifndef FORM_NUMBER_HPP
#define FORM_NUMBER_HPP

#include "form.hpp"

#include <tuple>

namespace twang::forms {

class number : public form {
public:
	enum type {
		INTEGER,
		DOUBLE,
		RATIO
	};

	number(type t);
	virtual ~number() = default;

	type number_type() const;

private:
	type m_type;
};

class integer : public number {
public:
	integer(long long value);
	~integer() = default;

	form* eval() override;
	std::string print() const override;

private:
	long long m_data;
};

class floating : public number {
public:
	floating(double value);
	~floating() = default;

	form* eval() override;
	std::string print() const override;

private:
	double m_data;
};

class ratio : public number {
public:
	ratio(long long n, unsigned long long d);
	ratio(const std::tuple<long long, unsigned long long>& r);
	~ratio() = default;

	form* eval() override;
	std::string print() const override;

private:
	std::tuple<long long, unsigned long long> m_data;
};

}

#endif
