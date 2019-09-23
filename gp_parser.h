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
static const int TGEFFECTBEND_MAX_POSITION_LENGTH = 12;
static const int TGEFFECTBEND_SEMITONE_LENGTH = 1;
static const int GP_BEND_SEMITONE = 25;
static const int GP_BEND_POSITION = 60;
static const int TGVELOCITIES_MIN_VELOCITY = 15;
static const int TGVELOCITIES_VELOCITY_INCREMENT = 16;

// Define struct to hold lyrics data
struct Lyric {
	std::int32_t from;
	std::string lyric;
};

// Define channel parameter struct
struct ChannelParam {
	std::string key;
	std::string value;
};

// Define channel struct
struct Channel {
	std::int32_t id;
	std::string name;
	std::int32_t program;
	std::int8_t volume;
	std::int8_t balance;
	std::int8_t chorus;
	std::int8_t reverb;
	std::int8_t phaser;
	std::int8_t tremolo;
	std::string bank;
	std::vector<ChannelParam> parameters;
};

// Define division struct
struct Division {
	std::int32_t enters;
	std::int32_t times;
};

// Define denominator struct
struct Denominator {
	std::int8_t value;
	Division division;
};

// Define time signature struct
struct TimeSignature {
	std::int8_t numerator;
	Denominator denominator;
};

// Define color struct
struct Color {
	std::uint8_t r;
	std::uint8_t g;
	std::uint8_t b;
};

// Define measure marker struct
struct Marker {
	std::int32_t measure;
	std::string title;
	Color color;	
};

// Define tempo struct
struct Tempo {
	std::int32_t value;
};

// Define measure header struct
struct MeasureHeader {
	std::int32_t number;
	std::int32_t start;
	Tempo tempo;
	bool repeatOpen;
	TimeSignature timeSignature;
	std::int8_t repeatClose;
	Marker marker;
	std::uint8_t repeatAlternative;
	std::string tripletFeel;
};

// Define tremolo point struct
struct TremoloPoint {
	std::int32_t pointPosition;
	std::int32_t pointValue;
};

// Define tremolo bar struct
struct TremoloBar {
	std::vector<TremoloPoint> points;
};

// Define bend point struct
struct BendPoint {
	std::int32_t pointPosition;
	std::int32_t pointValue;
};

// Define bend struct
struct Bend {
	std::vector<BendPoint> points;
};

// Define grace struct
struct Grace {
	std::uint8_t fret;
	std::int32_t dynamic;
	std::string transition;
	std::uint8_t duration;
	bool dead;
	bool onBeat;
};

// Define effect duration struct
struct EffectDuration {
	std::string value;
};

// Define tremolo picking struct
struct TremoloPicking {
	EffectDuration duration;
};

// Define harmonic struct
struct Harmonic {
	std::int32_t data;
	std::string type;
};

// Define trill struct
struct Trill {
	std::int8_t fret;
	EffectDuration duration;
};

// Define note effect struct
struct NoteEffect {
	bool fadeIn;
	bool vibrato;
	bool tapping;
	bool slapping;
	bool popping;
	bool deadNote;
	bool accentuatedNote;
	bool heavyAccentuatedNote;
	bool ghostNote;
	bool slide;
	bool hammer;
	bool letRing;
	bool palmMute;
	bool staccato;
	TremoloBar tremoloBar;
	TremoloPicking tremoloPicking;
	Bend bend;
	Grace grace;
	Harmonic harmonic;
	Trill trill;
};

// Define note struct
struct Note {
	std::int32_t string;
	NoteEffect effect;
	bool tiedNote;
	std::int8_t value;
	std::int32_t velocity;
};

// Define voice struct
struct Voice {
	std::vector<Note> notes;
	double duration;
	bool empty;
};

// Define stroke struct
struct Stroke {
	std::string direction;
	std::string value;
};

// Define guitar string struct
struct GuitarString {
	std::int32_t number;
	std::int32_t value;
};

// Define chord struct
struct Chord {
	std::string name;
	std::vector<GuitarString>* strings;
	std::vector<std::int32_t> frets;
};

// Define beat text struct
struct BeatText {
	std::string value;
};

// Define beat struct
struct Beat {
	std::int32_t start;
	std::vector<Voice> voices;
	Stroke stroke;
	BeatText text;
	Chord chord;
};

// Define measure struct
struct Measure {
	MeasureHeader *header;
	std::int32_t start;
	std::vector<Beat> beats;
	std::int8_t keySignature;
};

// Define track struct
struct Track {
	std::int32_t channelId;
	std::int32_t number;
	Lyric lyrics;
	std::string name;
	std::vector<GuitarString> strings;
	std::int32_t offset;
	Color color;
	std::vector<Measure> measures;
};

// Define duration struct
struct Duration {
	double value;
	bool dotted;
	bool doubleDotted;
	Division division;
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
	std::vector<MeasureHeader> measureHeaders;
	std::vector<Track> tracks;

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
	Color readColor();
	void readChannel(Track& track);
	void readMeasure(Measure& measure, Track& track, Tempo& tempo, std::int8_t keySignature);
	std::int32_t getLength(MeasureHeader& header);
	Beat& getBeat(Measure& measure, std::int32_t start);
	void readMixChange(Tempo& tempo);
	void readBeatEffects(Beat& beat, NoteEffect& noteEffect);
	void readTremoloBar(NoteEffect& effect);
	void readText(Beat& beat);
	void readChord(std::vector<GuitarString>& strings, Beat& beat);
	double getTime(Duration& duration);
	double readDuration(std::uint8_t flags);
	double readBeat(std::int32_t start, Measure& measure, Track& track, Tempo& tempo, std::size_t voiceIndex);
	Note readNote(GuitarString& string, Track& track, NoteEffect& effect);
	std::int8_t getTiedNoteValue(std::int32_t string, Track& track);
	void readNoteEffects(NoteEffect& noteEffect);
	void readBend(NoteEffect& effect);
	void readGrace(NoteEffect& effect);
	void readTremoloPicking(NoteEffect& effect);
	void readArtificialHarmonic(NoteEffect& effect);
	void readTrill(NoteEffect& effect);
};

std::int32_t numOfDigits(std::int32_t num);

}

#endif
