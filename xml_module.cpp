/* Copyright Phillip Potter, 2019 under MIT License */
#include <string>
#include <sstream>
#include "gp_parser.h"

namespace gp_parser {

template <class T>
void addObjectsToXML(const std::string& name, const std::vector<T>& objects, std::ostringstream& outputStream, std::int32_t indentLevel)
{
  if (objects.size() > 0) {
    addSpacingToXML(outputStream, indentLevel);
    outputStream << "<" << name << ">\n";
    for (auto i = 0; i < objects.size(); ++i)
      objects[i].addToXML(outputStream, indentLevel + 1);
    addSpacingToXML(outputStream, indentLevel);
    outputStream << "</" << name << ">\n";
  }
}

/* Calling this will provide a std::string which has the XML representing the
 * tab file used to construct the parser object */
std::string Parser::getXML() const
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
	outputStream << XML_SPACING << "<KeySignature>" << static_cast<std::int32_t>(globalKeySignature) << "</KeySignature>\n";

	// Output channels
	addObjectsToXML("Channels", channels, outputStream, 1);

	// Output measures
	outputStream << XML_SPACING << "<Measures>" << measures << "</Measures>\n";

	// Output track count
	outputStream << XML_SPACING << "<TrackCount>" << trackCount << "</TrackCount>\n";

	// Output measure headers
	addObjectsToXML("MeasureHeaders", measureHeaders, outputStream, 1);

	// Output tracks
    addObjectsToXML("Tracks", tracks, outputStream, 1);

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

void Lyric::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
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

void Channel::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
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
	outputStream << "<Bank>" << bank << "</Bank>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<IsPercussionChannel>" << (isPercussionChannel ? "true" : "false")
		     << "</IsPercussionChannel>\n";
	addObjectsToXML("ChannelParameters", parameters, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Channel>\n";
}

void ChannelParam::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
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

void MeasureHeader::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<MeasureHeader>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Number>" << number << "</Number>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Start>" << start << "</Start>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<RepeatOpen>" << (repeatOpen ? "true" : "false") << "</RepeatOpen>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<RepeatClose>" << static_cast<std::int32_t>(repeatClose) << "</RepeatClose>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<RepeatAlternative>" << static_cast<std::uint32_t>(repeatAlternative) << "</RepeatAlternative>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<TripletFeel>" << tripletFeel << "</TripletFeel>\n";
	tempo.addToXML(outputStream, indentLevel + 1);
	timeSignature.addToXML(outputStream, indentLevel + 1);
	marker.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</MeasureHeader>\n";
}

void Tempo::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Tempo>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Tempo>\n";
}

void TimeSignature::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<TimeSignature>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Numerator>" << static_cast<std::int32_t>(numerator) << "</Numerator>\n";
	denominator.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</TimeSignature>\n";
}

void Denominator::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Denominator>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << static_cast<std::int32_t>(value) << "</Value>\n";
	division.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Denominator>\n";
}

void Division::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Division>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Enters>" << enters << "</Enters>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Times>" << times << "</Times>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Division>\n";
}

void Marker::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Marker>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Measure>" << measure << "</Measure>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Title>" << title << "</Title>\n";
	color.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Marker>\n";
}

void Color::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Color>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Red>" << static_cast<uint32_t>(r) << "</Red>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Green>" << static_cast<uint32_t>(g) << "</Green>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Blue>" << static_cast<uint32_t>(b) << "</Blue>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Color>\n";
}

void Track::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Track>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<ChannelId>" << channelId << "</ChannelId>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Number>" << number << "</Number>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Name>" << name << "</Name>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Offset>" << offset << "</Offset>\n";
	lyrics.addToXML(outputStream, indentLevel + 1);
	color.addToXML(outputStream, indentLevel + 1);
	addObjectsToXML("Strings", strings, outputStream, indentLevel + 1);
	addObjectsToXML("Measures", measures, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Track>\n";
}

void GuitarString::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<String>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Number>" << number << "</Number>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</String>\n";
}

void Measure::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Measure>\n";

	header->addToXML(outputStream, indentLevel + 1);
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Start>" << start << "</Start>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<KeySignature>" << static_cast<std::int32_t>(keySignature) << "</KeySignature>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Clef>" << clef << "</Clef>\n";
    addObjectsToXML("Beats", beats, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Measure>\n";
}

void Beat::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Beat>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Start>" << start << "</Start>\n";
	text.addToXML(outputStream, indentLevel + 1);
	stroke.addToXML(outputStream, indentLevel + 1);
	chord.addToXML(outputStream, indentLevel + 1);
	addObjectsToXML("Voices", voices, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Beat>\n";
}

void BeatText::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<BeatText>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</BeatText>\n";
}

void Stroke::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Stroke>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Direction>" << direction << "</Direction>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Stroke>\n";
}

void Chord::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Chord>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Name>" << name << "</Name>\n";
	if (strings != nullptr) {
      addObjectsToXML("Strings", *strings, outputStream, indentLevel + 1);
    }
	if (frets.size() > 0) {
		addSpacingToXML(outputStream, indentLevel + 1);
		outputStream << "<Frets>\n";
		for (auto i = 0; i < frets.size(); ++i) {
			addSpacingToXML(outputStream, indentLevel + 2);
			outputStream << "<Fret>" << frets[i] << "</Fret>\n";
		}
		addSpacingToXML(outputStream, indentLevel + 1);
		outputStream << "</Frets>\n";
	}

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Chord>\n";
}

void Voice::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Voice>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Empty>" << (empty ? "true" : "false") << "</Empty>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Duration>" << duration << "</Duration>\n";
	addObjectsToXML("Notes", notes, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Voice>\n";
}

void Note::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Note>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<String>" << string << "</String>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<TiedNote>" << (tiedNote ? "true" : "false") << "</TiedNote>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << static_cast<std::int32_t>(value) << "</Value>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Velocity>" << velocity << "</Velocity>\n";
	effect.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Note>\n";
}

void NoteEffect::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Effect>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<FadeIn>" << (fadeIn ? "true" : "false") << "</FadeIn>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Vibrato>" << (vibrato ? "true" : "false") << "</Vibrato>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Tapping>" << (tapping ? "true" : "false") << "</Tapping>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Slapping>" << (slapping ? "true" : "false") << "</Slapping>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Popping>" << (popping ? "true" : "false") << "</Popping>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<DeadNote>" << (deadNote ? "true" : "false") << "</DeadNote>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<AccentuatedNote>" << (accentuatedNote ? "true" : "false") << "</AccentuatedNote>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<HeavyAccentuatedNote>" << (heavyAccentuatedNote ? "true" : "false") << "</HeavyAccentuatedNote>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<GhostNote>" << (ghostNote ? "true" : "false") << "</GhostNote>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Slide>" << (slide ? "true" : "false") << "</Slide>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Hammer>" << (hammer ? "true" : "false") << "</Hammer>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<LetRing>" << (letRing ? "true" : "false") << "</LetRing>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<PalmMute>" << (palmMute ? "true" : "false") << "</PalmMute>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Staccato>" << (staccato ? "true" : "false") << "</Staccato>\n";
	tremoloBar.addToXML(outputStream, indentLevel + 1);
	tremoloPicking.addToXML(outputStream, indentLevel + 1);
	bend.addToXML(outputStream, indentLevel + 1);
	grace.addToXML(outputStream, indentLevel + 1);
	harmonic.addToXML(outputStream, indentLevel + 1);
	trill.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Effect>\n";
}

void TremoloBar::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<TremoloBar>\n";

	addObjectsToXML("Points", points, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</TremoloBar>\n";
}

void TremoloPoint::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<TremoloPoint>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<PointPosition>" << pointPosition << "</PointPosition>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<PointValue>" << pointValue << "</PointValue>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</TremoloPoint>\n";
}

void TremoloPicking::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<TremoloPicking>\n";

	duration.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</TremoloPicking>\n";
}

void EffectDuration::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<EffectDuration>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Value>" << value << "</Value>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</EffectDuration>\n";
}

void Bend::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Bend>\n";

	addObjectsToXML("BendPoints", points, outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Bend>\n";
}

void BendPoint::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<BendPoint>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<PointPosition>" << pointPosition << "</PointPosition>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<PointValue>" << pointValue << "</PointValue>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</BendPoint>\n";
}

void Grace::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Grace>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Fret>" << static_cast<std::uint32_t>(fret) << "</Fret>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Dynamic>" << dynamic << "</Dynamic>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Transition>" << transition << "</Transition>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Duration>" << static_cast<std::uint32_t>(duration) << "</Duration>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Dead>" << (dead ? "true" : "false") << "</Dead>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<OnBeat>" << (onBeat ? "true" : "false") << "</OnBeat>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Grace>\n";
}

void Harmonic::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Harmonic>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Type>" << type << "</Type>\n";
	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Data>" << data << "</Data>\n";

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Harmonic>\n";
}

void Trill::addToXML(std::ostringstream& outputStream, std::int32_t indentLevel) const
{
	addSpacingToXML(outputStream, indentLevel);
	outputStream << "<Trill>\n";

	addSpacingToXML(outputStream, indentLevel + 1);
	outputStream << "<Fret>" << static_cast<std::int32_t>(fret) << "</Fret>\n";
	duration.addToXML(outputStream, indentLevel + 1);

	addSpacingToXML(outputStream, indentLevel);
	outputStream << "</Trill>\n";
}

}
