/* Copyright Phillip Potter, 2019 under MIT License
 * Based upon https://github.com/juliangruber/parse-gp5 (also MIT) */
#include <stdexcept>
#include <fstream>
#include <iterator>
#include "gp_parser.h"

namespace gp_parser {

/* This constructor takes a Guitar Pro file and reads it into the internal
 * vector for further use */
Parser::Parser(const char *filePath)
{
	// Open file
	if (filePath == nullptr)
		throw std::logic_error("Null file path passed to constructor");
	std::ifstream file;
	file.open(filePath, std::ifstream::in | std::ifstream::binary);

	// Initialise vector
	fileBuffer = std::vector<char>(
		     std::istreambuf_iterator<char>(file),
		     {}
		     );

	// Close file
	file.close();
}

/* This reads an unsigned byte from the file buffer and increments the
 * position at the same time */
std::uint8_t Parser::readUnsignedByte()
{
	return static_cast<uint8_t>(fileBuffer[bufferPosition++]);
}

/* This reads a signed byte from the file buffer and increments the
 * position at the same time */
std::int8_t Parser::readByte()
{
	return static_cast<int8_t>(fileBuffer[bufferPosition++]);
}

/* This reads a signed 32-bit integer from the file buffer in little-endian
 * mode and increments the position at the same time */
std::int32_t Parser::readInt()
{
	int32_t returnVal = static_cast<int32_t>(
			    ((fileBuffer[bufferPosition + 3] & 0xFF) << 24) |
			    ((fileBuffer[bufferPosition + 2] & 0xFF) << 16) |
			    ((fileBuffer[bufferPosition + 1] & 0xFF) << 8) |
			    (fileBuffer[bufferPosition] & 0xFF)
			    );
	bufferPosition += 4;

	return returnVal;
}

/* This just moves the position past 'n' number of bytes in the file buffer */
void Parser::skip(std::size_t n)
{
	bufferPosition += n;
}

}
