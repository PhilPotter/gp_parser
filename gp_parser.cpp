/* Copyright Phillip Potter, 2019 under MIT License
 * Based upon https://github.com/juliangruber/parse-gp5 (also MIT) */
#include <stdexcept>
#include <fstream>
#include <iterator>
#include <regex>
#include "gp_parser.h"
#include <iostream>

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

	// Parse version and check it is supported
	readVersion();
	if (!isSupportedVersion(version))
		throw std::logic_error("Unsupported version");

	// Parse out major and minor version numbers
	std::regex majorAndMinorExp("(\\d+)\\.(\\d+)");
	major = std::stoi(std::regex_replace(
			  version,
			  majorAndMinorExp,
			  "$1",
			  std::regex_constants::format_no_copy));
	minor = std::stoi(std::regex_replace(
			  version,
			  majorAndMinorExp,
			  "$2",
			  std::regex_constants::format_no_copy));

	// Read attributes of tab file
	title = readStringByteSizeOfInteger();
	subtitle = readStringByteSizeOfInteger();
	artist = readStringByteSizeOfInteger();
	album = readStringByteSizeOfInteger();
	lyricsAuthor = readStringByteSizeOfInteger();
	musicAuthor = readStringByteSizeOfInteger();
	copyright = readStringByteSizeOfInteger();
	tab = readStringByteSizeOfInteger();
	instructions = readStringByteSizeOfInteger();
	auto commentLen = readInt();
	for (auto i = 0; i < commentLen; ++i)
		comments.push_back(readStringByteSizeOfInteger());

	// Read lyrics data
	lyricTrack = readInt();
	lyric = readLyrics();

	// Read page setup
	readPageSetup();

	// Read tempo value
	tempoValue = readInt();

	if (versionIndex > 0)
		skip(1);

	// Read key signature
	auto keySignature = readKeySignature();
	
	skip(3);

	// Octave
	readByte();

	// Read channels
	channels = readChannels();

	skip(42);

	// Read measures and track count info
	measures = readInt();
	trackCount = readInt();

	// Read measure headers
	TimeSignature timeSignature;
	timeSignature.numerator = 4;
	timeSignature.denominator.value = QUARTER;
	timeSignature.denominator.division.enters = 1;
	timeSignature.denominator.division.times = 1;
	for (auto i = 0; i < measures; ++i) {
		if (i > 0)
			skip(1);
		std::uint8_t flags = readUnsignedByte();
		MeasureHeader header;
		header.number = i + 1;
		header.start = 0;
		header.tempo = 120;
		header.repeatOpen = (flags & 0x04) != 0;
		if ((flags & 0x01) != 0)
			timeSignature.numerator = readByte();
		if ((flags & 0x02) != 0)
			timeSignature.denominator.value = readByte();
		header.timeSignature = timeSignature;
		if ((flags & 0x08) != 0)
			header.repeatClose = (readByte() & 0xFF) - 1;
		if ((flags & 0x20) != 0) {
			header.marker.measure = header.number;
			header.marker.title = readStringByteSizeOfInteger();
			header.marker.color = readColor();
		}
		if ((flags & 0x10) != 0)
			header.repeatAlternative = readUnsignedByte();
		if ((flags & 0x40) != 0) {
			keySignature = readKeySignature();
			skip(1);
		}
		if ((flags & 0x01) != 0 || (flags & 0x02) != 0)
			skip(4);
		if ((flags & 0x10) == 0)
			skip(1);
		std::int8_t tripletFeel = readByte();
		if (tripletFeel == 1)
			header.tripletFeel = "eigth";
		else if (tripletFeel == 2)
			header.tripletFeel = "sixteents";
		else
			header.tripletFeel = "none";

		// Push header to vector
		measureHeaders.push_back(header);
	}
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

/* This returns a string from the file buffer, but using an integer before it
 * to tell it the total number of bytes to read - the initial byte that is
 * read still gives the string length */
std::string Parser::readStringByteSizeOfInteger()
{
	return readStringByte(readInt() - 1);
}

std::string Parser::readStringInteger()
{
	return readString(readInt());
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
	for (auto i = 0; i < versionsCount; ++i) {
		if (version.compare(VERSIONS[i]) == 0) {
			versionIndex = i;
			return true;
		}
	}
	return false;
}

/* This reads lyrics data */
Lyric Parser::readLyrics()
{
	Lyric lyric;
	lyric.from = readInt();
	lyric.lyric = readStringInteger();

	for (auto i = 0; i < 4; ++i) {
		readInt();
		readStringInteger();
	}

	return lyric;
}

/* This reads the page setup data */
void Parser::readPageSetup()
{
	skip(versionIndex > 0 ? 49 : 30);
	for (auto i = 0; i < 11; ++i) {
		skip(4);
		readStringByte(0);
	}
}

/* This reads the key signature */
std::int8_t Parser::readKeySignature()
{
	auto keySignature = readByte();
	if (keySignature < 0)
		keySignature = 7 - keySignature;

	return keySignature;
}

/* This reads the channel attributes data */
std::vector<Channel> Parser::readChannels()
{
	std::vector<Channel> channels;
	for (auto i = 0; i < 64; ++i) {
		Channel channel;
		channel.program = readInt();
		channel.volume = readByte();
		channel.balance = readByte();
		channel.chorus = readByte();
		channel.reverb = readByte();
		channel.phaser = readByte();
		channel.tremolo = readByte();
		channel.bank =
			i == 9 ?
			"default percussion bank" :
			"default bank";
		if (channel.program < 0)
			channel.program = 0;
		channels.push_back(channel);
		skip(2);
	}

	return channels;
}

// Read a color value
Color Parser::readColor()
{
	Color c;
	c.r = readUnsignedByte();
	c.g = readUnsignedByte();
	c.b = readUnsignedByte();
	skip(1);

	return c;
}

}
