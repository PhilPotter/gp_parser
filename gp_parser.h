/* Copyright Phillip Potter, 2019 under MIT License
 * Based upon https://github.com/juliangruber/parse-gp5 (also MIT) */
#ifndef GP_PARSER
#define GP_PARSER

#include <cstddef>
#include <cstdint>
#include <vector>

namespace gp_parser {

class Parser {
public:
	Parser(const char *filePath);
private:
	// Private class properties
	static constexpr const char *versions[] = {
		"FICHIER GUITAR PRO v5.00",
		"FICHIER GUITAR PRO v5.10"
	};
	static const int QUARTER_TIME = 960;
	static const int QUARTER = 4;

	// Private member properties
	std::vector<char> fileBuffer;
	std::size_t bufferPosition = 0;

	// Private member functions
	std::uint8_t readUnsignedByte();
	std::int8_t readByte();
	std::int32_t readInt();
	void skip(std::size_t n);
};

}

#endif
