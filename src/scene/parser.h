#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <scene/scene.h>

namespace parser {

	/*defined classes*/
	class parser_t;

	class parser_t
	{
	 	public:
			parser_t(){};
			parser_t(const std::string& filename, 
					scene_t& scene);

			int read(const std::string& filename,
					scene_t& scene);

			void clear();
	};
}

#endif



