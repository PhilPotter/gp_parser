/* Copyright Phillip Potter, 2019 under MIT License
 * Based upon https://github.com/juliangruber/parse-gp5 (also MIT) */
#ifndef GP_PARSER
#define GP_PARSER

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

namespace gp_parser {

// Supported versions and other data
static const char *VERSIONS[] = {
	"FICHIER GUITAR PRO v5.00",
	"FICHIER GUITAR PRO v5.10"
};
static const int QUARTER_TIME = 960;
static const int QUARTER = 4;

class Parser {
public:
	Parser(const char *filePath);
private:
	// Private member properties
	std::vector<char> fileBuffer;
	std::size_t bufferPosition = 0;
	std::string version;
	std::size_t versionIndex;

	// Private member functions for reading low-level file data
	std::uint8_t readUnsignedByte();
	std::int8_t readByte();
	std::int32_t readInt();
	std::string readString(size_t size);
	std::string readString(size_t size, size_t len);
	std::string readStringByte(size_t size);
	void skip(std::size_t n);

	// Private member functions for parsing higher-level file data
	void readVersion();
	bool isSupportedVersion(std::string& version);
};

}

#endif
