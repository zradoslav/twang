#ifndef FORM_H
#define FORM_H

#include <variant>

#include <list>
#include <map>
#include <set>
#include <vector>

// // namespace lyzp::form {

// typedef IFn
// static const std::map<std::string, 

enum form_type {
	SYMBOL,
	STRING,
	NUMBER,
	CHAR,
	NIL,
	BOOLEAN,
	SYMVAL,
	KEYWORD,
	LIST,
	VECTOR,
	MAP,
	SET 
};

class form {
public:
	form(form_type t = NIL) : m_type(t) {}

	virtual ~form() {}

	form_type type() const { return m_type; }

	virtual form* eval() {} //= 0;

private:
	form_type m_type;
};

/* symbols */

class symbol : public form {
public:
	symbol(const std::string& name)
		: form(SYMBOL), m_name(name)
	{}
	~symbol() = default;

	form* eval() override {
		// TODO: resolve from env
	}

	std::string name() const { return m_name; }

private:
	std::string m_name;
};

/* literals */

class string : public form {
public:
	string(const std::string& value)
		: form(STRING), m_data(value)
	{}
	~string() = default;

	form* eval() { return this; }

	std::string value() const { return m_data; }

private:
	std::string m_data;
};

class number : public form {
public:
	/* remove asap */
	typedef int                          integer;
	typedef std::tuple<integer, integer> ratio;

	enum type {
		INTEGER = 0,
		FLOAT,
		// RATIO
	};

	number(int value)
		: form(NUMBER), m_num_type(INTEGER)
		, m_data(value)
	{}
	number(double value)
		: form(NUMBER), m_num_type(FLOAT)
		, m_data(value)
	{}
	// number(int numer, int denom)
	// 	: form(NUMBER), m_num_type(RATIO),
	// 	, m_data(std::make_tuple(numer, denom))
	// {}
	~number() = default;

	form* eval() override { return this; }

	std::variant<int, double> value() const {
		switch (m_num_type) {
			case INTEGER: return std::get<int>(m_data);
			case FLOAT: return std::get<double>(m_data);
		}
	}

private:
	type m_num_type;
	std::variant<int, double> m_data;
};

class nil : public form {
public:
	nil() : form(NIL) {}
	~nil() = default;

	form* eval() override { return this; }
};

class boolean : public form {
public:
	boolean(bool value)
		: form(BOOLEAN), m_data(value)
	{}
	~boolean() = default;

	form* eval() override { return this; }

	bool value() const { return m_data; }

private:
	bool m_data;
};

class keyword : public form {
public:
	keyword(const std::string name)
		: form(KEYWORD), m_name(name)
	{}
	~keyword() = default;

	form* eval() override { return this; }

	std::string name() const { return m_name; }

private:
	std::string m_name;
};

// TODO

/* sequences */

class list : public form {
public:
	list() : form(LIST) {}
	~list() = default;

	form* eval() override {
		// TODO
	}

	std::list<form*> collection() const { return m_data; }
	void append(form* f) { m_data.push_back(f); }

private:
	std::list<form*> m_data;
};

class vector : public form {
public:
	vector() : form(VECTOR) {}
	~vector() = default;

	form* eval() override {
		// TODO
	}

	std::vector<form*> collection() const { return m_data; }
	void append(form* f) { m_data.push_back(f); }

private:
	std::vector<form*> m_data;
};

class map : public form {
public:
	map() : form(MAP) {}
	~map() = default;

	form* eval() override {
		// TODO
	}

	std::map<form*, form*> collection() const { return m_data; }
	void add(form* key, form* value) {
		m_data[key] = value;
	}

private:
	std::map<form*, form*> m_data;
};

class set : public form {
public:
	set() : form(SET) {}
	~set() = default;

	form* eval() override {
		// TODO
	}

	std::set<form*> collection() const { return m_data; }
	void add(form* value) {
		m_data.insert(value);
	}

private:
	std::set<form*> m_data;
};

#endif
