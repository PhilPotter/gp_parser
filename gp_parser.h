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

// Define struct to hold lyrics data
struct Lyric {
	std::int32_t from;
	std::string lyric;
};

struct Channel {
	std::int32_t program;
	std::int8_t volume;
	std::int8_t balance;
	std::int8_t chorus;
	std::int8_t reverb;
	std::int8_t phaser;
	std::int8_t tremolo;
	std::string bank;
};

class Parser {
public:
	Parser(const char *filePath);
private:
	// Private member properties
	std::vector<char> fileBuffer;
	std::size_t bufferPosition = 0;
	std::string version;
	std::size_t versionIndex;
	std::int32_t major;
	std::int32_t minor;
	std::string title;
	std::string subtitle;
	std::string artist;
	std::string album;
	std::string lyricsAuthor;
	std::string musicAuthor;
	std::string copyright;
	std::string tab;
	std::string instructions;
	std::vector<std::string> comments;
	std::int32_t lyricTrack;
	Lyric lyric;
	std::int32_t tempoValue;
	std::vector<Channel> channels;
	std::int32_t measures;
	std::int32_t trackCount;

	// Private member functions for reading low-level file data
	std::uint8_t readUnsignedByte();
	std::int8_t readByte();
	std::int32_t readInt();
	std::string readString(std::size_t size);
	std::string readString(std::size_t size, std::size_t len);
	std::string readStringByte(std::size_t size);
	std::string readStringByteSizeOfInteger();
	std::string readStringInteger();
	void skip(std::size_t n);

	// Private member functions for parsing higher-level file data
	void readVersion();
	bool isSupportedVersion(std::string& version);
	Lyric readLyrics();
	void readPageSetup();
	std::int8_t readKeySignature();
	std::vector<Channel> readChannels();
};

}

#endif
