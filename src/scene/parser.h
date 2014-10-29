#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>

namespace parser {

	/*defined classes*/
	class parser;

	class parser
	{
	 	public:
			parser(){};
			parser(const parser& other): {};
			parser(const std::string& filename);

			int read(const std::string& filename);

			void clear();
	};
}

#endif



