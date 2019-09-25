/* Copyright Phillip Potter, 2019 under MIT License */
#include <string>
#include <sstream>
#include "gp_parser.h"

namespace gp_parser {

/* Calling this will provide a std::string which has the XML representing the
 * tab file used to construct the parser object */
std::string Parser::getXML()
{
	// Declare output stream
	std::ostringstream outputStream;

	// Output XML declaration to stream
	outputStream << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";

	// Output opening tag
	outputStream << "<TabFile>\n";

	// Begin outputting state
	outputStream << XML_SPACING << "<Version>\n";
	outputStream << XML_SPACING << XML_SPACING << "<Major>" << major << "</Major>\n";
	outputStream << XML_SPACING << XML_SPACING << "<Minor>" << minor << "</Minor>\n";
	outputStream << XML_SPACING << "</Version>\n";
	outputStream << XML_SPACING << "<Title>" << title << "</Title>\n";
	outputStream << XML_SPACING << "<Subtitle>" << subtitle << "</Subtitle>\n";
	outputStream << XML_SPACING << "<Artist>" << artist << "</Artist>\n";
	outputStream << XML_SPACING << "<Album>" << album << "</Album>\n";
	outputStream << XML_SPACING << "<LyricsAuthor>" << lyricsAuthor << "</LyricsAuthor>\n";
	outputStream << XML_SPACING << "<MusicAuthor>" << musicAuthor << "</MusicAuthor>\n";
	outputStream << XML_SPACING << "<Copyright>" << copyright << "</Copyright>\n";
	outputStream << XML_SPACING << "<Tab>" << tab << "</Tab>\n";
	outputStream << XML_SPACING << "<Instructions>" << instructions << "</Instructions>\n";

	// Output comments
	if (comments.size() > 0) {
		outputStream << XML_SPACING << "<Comments>\n";
		for (auto i = 0; i < comments.size(); ++i) {
			outputStream << XML_SPACING << XML_SPACING;
			outputStream << "<Comment>" << comments[i] << "</Comment>\n";
		}
		outputStream << XML_SPACING << "</Comments>\n";
	}

	// Output lyric
	lyric.addToXML(outputStream, 1);

	// Output tempo value
	outputStream << XML_SPACING << "<TempoValue>" << tempoValue << "</TempoValue>\n";

	// Output key signature
	outputStream << XML_SPACING << "<KeySignature>" << globalKeySignature << "</KeySignature>\n";

	// Output channels
	if (channels.size() > 0) {
		outputStream << XML_SPACING << "<Channels>\n";
		for (auto i = 0; i < channels.size(); ++i)
			channels[i].addToXML(outputStream, 2);
		outputStream << XML_SPACING << "</Channels>\n";
	}

	// Output measures
	outputStream << XML_SPACING << "<Measures>" << measures << "</Measures>\n";

	// Output track count
	outputStream << XML_SPACING << "<TrackCount>" << trackCount << "</TrackCount>\n";

	// Output measure headers
	if (measureHeaders.size() > 0) {
		outputStream << XML_SPACING << "<MeasureHeaders>";
		for (auto i = 0; i < measureHeaders.size(); ++i)
			measureHeaders.addToXML(outputStream, 2);
		outputStream << XML_SPACING << "</MeasureHeaders>";
	}

	// Output closing tag
	outputStream << "</TabFile>\n";

	return outputStream.str();
}

/* This function allows us to add an arbitrary number of indents to our XML stream */
void addSpacingToXML(std::ostringstream& outputStream, std::int32_t indentLevel)
{
	for (auto i = 0; i < indentLevel; ++i)
		outputStream << XML_SPACING;
}

/* Below are all the struct-specific addToXML() functions */

void Lyric::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel)
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<LyricInfo>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<From>" << from << "</From>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Lyric>" << lyric << "</Lyric>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</LyricInfo>\n";
}

void Channel::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel)
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Channel>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Id>" << id << "</Id>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Name>" << name << "</Name>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Program>" << program << "</Program>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Volume>" << static_cast<std::int32_t>(volume) << "</Volume>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Balance>" << static_cast<std::int32_t>(balance) << "</Balance>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Chorus>" << static_cast<std::int32_t>(chorus) << "</Chorus>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Reverb>" << static_cast<std::int32_t>(reverb) << "</Reverb>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Phaser>" << static_cast<std::int32_t>(phaser) << "</Phaser>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Tremolo>" << static_cast<std::int32_t>(tremolo) << "</Tremolo>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Bank>" << phaser << "</Bank>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<IsPercussionChannel>" << (isPercussionChannel ? "true" : "false")
		     << "</IsPercussionChannel>\n";
	for (auto i = 0; i < parameters.size(); ++i)
		parameters[i].addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Channel>\n";
}

void ChannelParam::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel)
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<ChannelParam>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Key>" << key << "</Key>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</ChannelParam>\n";
}

}
