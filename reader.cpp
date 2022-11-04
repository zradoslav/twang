#include <cctype>
#include <fstream>
#include <cassert>

#include "reader.h"
#include "twang.tab.hh"
#include "scanner.h"

namespace twang {

reader::~reader() {
	delete m_scanner;
	delete m_parser;
} // end destructor

// -----------------------------------------------------------------------------

void reader::parseFromFile(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.good()) {
		std::exit(1);
	} // end if
	parseFromStream(file);
} // end method

// -----------------------------------------------------------------------------

void reader::parseFromString(const std::string &str) {
	std::istringstream iss(str);
	parseFromStream(iss);
} // end method

// -----------------------------------------------------------------------------

void reader::parseFromStream(std::istream &stream) {
	if (!stream.good() && stream.eof()) {
		return;
	} // end if
	
	delete m_scanner;
	try {
		m_scanner = new scanner(&stream);
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate scanner: (" <<
				ba.what() << "), exiting!!\n";
		std::exit(1);
	} // end catch

	delete m_parser ;
	try {
		m_parser = new parser(*m_scanner, *this);
	} catch (std::bad_alloc &ba) {
		std::cerr << "Failed to allocate parser: (" <<
				ba.what() << "), exiting!!\n";
		std::exit(1);
	} // end catch
	
	if (m_parser->parse() != 0) {
		std::cout << "Parse failed.\n";
		std::exit(1);
	} else {
		std::cout << "Parse succeed.\n";
	} // end else

} // end method

} // end namespace
