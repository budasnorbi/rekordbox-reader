#pragma once
#include <vector>

#ifdef READER_EXPORTS
#define READER_API __declspec(dllexport)
#else
#define READER_API __declspec(dllimport)
#endif

struct Changes {
	double d1ChannelFader = 0.0 ;
	double d1CfxKnob = 0.0;
	double d1LowFilter = 0.0;
	double d1MidFilter = 0.0;
	double d1HighFilter = 0.0;
	double d1TrimKnob = 0.0;
	double d1Tempo = 0.0;
	uint32_t d1SongId = 0;
	int d1IsPlaying = -1;
	double d1CurrentTime = 0.0;
	double d1CalculatedTempo = 0.0;
	double d1CalculatedFristBeat = 0.0;

	double d2ChannelFader = 0.0;
	double d2CfxKnob = 0.0;
	double d2LowFilter = 0.0;
	double d2MidFilter = 0.0;
	double d2HighFilter = 0.0;
	double d2TrimKnob = 0.0;
	double d2Tempo = 0.0;
	uint32_t d2SongId = 0;
	int d2IsPlaying = -1;
	double d2CurrentTime = 0.0;
	double d2CalculatedTempo = 0.0;
	double d2CalculatedFristBeat = 0.0;

	double crossfader = 0.0;
};


extern "C" READER_API Changes * getData();

