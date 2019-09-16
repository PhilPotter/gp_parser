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

	// Begin parsing file data
	readVersion();
	if (!isSupportedVersion(version))
		throw std::logic_error("Unsupported version");
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
	auto returnVal = static_cast<int32_t>(
			    ((fileBuffer[bufferPosition + 3] & 0xFF) << 24) |
			    ((fileBuffer[bufferPosition + 2] & 0xFF) << 16) |
			    ((fileBuffer[bufferPosition + 1] & 0xFF) << 8) |
			    (fileBuffer[bufferPosition] & 0xFF)
			    );
	bufferPosition += 4;

	return returnVal;
}

/* This version of the function takes no 'len' parameter and merely forwards
 * through to the full method by setting 'len' to be equal to 'size' */
std::string Parser::readString(size_t size)
{
	return readString(size, size);
}

/* This returns a string from the file buffer, in the general case by reading
 * 'size' bytes from the file buffer then converting it to a string of 'len'
 * bytes */
std::string Parser::readString(size_t size, size_t len)
{
	// Work out number of bytes to read
	auto bytesToRead = size > 0 ? size : len;

	// Read this number of bytes from the file buffer
	auto bytes = std::vector<char>(bytesToRead);
	std::copy(fileBuffer.begin() + bufferPosition,
		  fileBuffer.begin() + bufferPosition + bytesToRead,
		  bytes.begin());

	// Increment position
	bufferPosition += bytesToRead;

	// Convert to string and return
	return std::string(bytes.begin(),
			   len >= 0 && len <= bytesToRead ?
			   (bytes.begin() + len) : (bytes.begin() + size));
}

/* This returns a string from the file buffer, but using a byte before it to
 * tell it the length of the string */
std::string Parser::readStringByte(size_t size)
{
	return readString(size, readUnsignedByte());
}

/* This just moves the position past 'n' number of bytes in the file buffer */
void Parser::skip(std::size_t n)
{
	bufferPosition += n;
}

/* This reads the version data from the file buffer */
void Parser::readVersion()
{
	version = readStringByte(30);
}

/* This checks if the supplied version is supported by the parser */
bool Parser::isSupportedVersion(std::string& version)
{
	auto versionsCount = sizeof(VERSIONS) / sizeof(const char *);
	for (size_t i; i < versionsCount; ++i) {
		if (version.compare(VERSIONS[i]) == 0) {
			versionIndex = i;
			return true;
		}
	}
	return false;
}

}
