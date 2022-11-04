#ifndef TWANG_READER_H
#define TWANG_READER_H

#include <string>
#include <cstddef>
#include <iostream>
#include <istream>
#include <sstream>

namespace twang {

class scanner;
class parser;

class reader {
friend class parser;

public:
	reader() :
		m_parser(nullptr),
		m_scanner(nullptr) {}

	virtual ~reader();

	void parseFromFile(const std::string &filename);
	void parseFromString(const std::string &str);
	void parseFromStream(std::istream &stream);

private:
	parser* m_parser;
	scanner* m_scanner;

	void readCallback(const std::string &type, const std::string &text) {
		std::cout << type << ": " << text << '\n';
	}
};

}
#endif

