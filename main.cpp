#include "reader.h"

#include <iostream>
#include <readline/readline.h>

#include "config.h"

int main(int argc, char* argv[])
{
	twang::reader r;
	r.parseFromFile(argv[1]);
	
	return 0;
}
