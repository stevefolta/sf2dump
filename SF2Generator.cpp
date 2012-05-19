#include "SF2Generator.h"
#include <stdio.h>	// just for NULL; where does that live really?

static const SF2Generator generators[] = {
	{ "startAddrsOffset", SF2Generator::Short },
	{ "endAddrsOffset", SF2Generator::Short },
	{ "startloopAddrsOffset", SF2Generator::Short },
	{ "endloopAddrsOffset", SF2Generator::Short },
	{ "startAddrsCoarseOffset", SF2Generator::Short },
	{ "modLfoToPitch", SF2Generator::Short },
	{ "vibLfoToPitch", SF2Generator::Short },
	{ "modEnvToPitch", SF2Generator::Short },
	{ "initialFilterFc", SF2Generator::Short },
	{ "initialFilterQ", SF2Generator::Short },
	{ "modLfoToFilterFc", SF2Generator::Short },
	{ "modEnvToFilterFc", SF2Generator::Short },
	{ "endAddrsCoarseOffset", SF2Generator::Short },
	{ "modLfoToVolume", SF2Generator::Short },
	{ "unused1", SF2Generator::Short },
	{ "chorusEffectsSend", SF2Generator::Short },
	{ "reverbEffectsSend", SF2Generator::Short },
	{ "pan", SF2Generator::Short },
	{ "unused2", SF2Generator::Short },
	{ "unused3", SF2Generator::Short },
	{ "unused4", SF2Generator::Short },
	{ "delayModLFO", SF2Generator::Short },
	{ "freqModLFO", SF2Generator::Short },
	{ "delayVibLFO", SF2Generator::Short },
	{ "freqVibLFO", SF2Generator::Short },
	{ "delayModEnv", SF2Generator::Short },
	{ "attackModEnv", SF2Generator::Short },
	{ "holdModEnv", SF2Generator::Short },
	{ "decayModEnv", SF2Generator::Short },
	{ "sustainModEnv", SF2Generator::Short },
	{ "releaseModEnv", SF2Generator::Short },
	{ "keynumToModEnvHold", SF2Generator::Short },
	{ "keynumToModEnvDecay", SF2Generator::Short },
	{ "delayVolEnv", SF2Generator::Short },
	{ "attackVolEnv", SF2Generator::Short },
	{ "holdVolEnv", SF2Generator::Short },
	{ "decayVolEnv", SF2Generator::Short },
	{ "sustainVolEnv", SF2Generator::Short },
	{ "releaseVolEnv", SF2Generator::Short },
	{ "keynumToVolEnvHold", SF2Generator::Short },
	{ "keynumToVolEnvDecay", SF2Generator::Short },
	{ "instrument", SF2Generator::Word },
	{ "reserved1", SF2Generator::Short },
	{ "keyRange", SF2Generator::Range },
	{ "velRange", SF2Generator::Range },
	{ "startloopAddrsCoarseOffset", SF2Generator::Short },
	{ "keynum", SF2Generator::Short },
	{ "velocity", SF2Generator::Short },
	{ "initialAttenuation", SF2Generator::Short },
	{ "reserved2", SF2Generator::Short },
	{ "endloopAddrsCoarseOffset", SF2Generator::Short },
	{ "coarseTune", SF2Generator::Short },
	{ "fineTune", SF2Generator::Short },
	{ "sampleID", SF2Generator::Word },
	{ "sampleModes", SF2Generator::Word },
	{ "reserved3", SF2Generator::Short },
	{ "scaleTuning", SF2Generator::Short },
	{ "exclusiveClass", SF2Generator::Short },
	{ "overridingRootKey", SF2Generator::Short },
	{ "unused5", SF2Generator::Short },
	{ "endOper", SF2Generator::Short },
	};
static const int numGenerators = sizeof(generators) / sizeof(generators[0]);


const SF2Generator* GeneratorFor(unsigned short index)
{
	if (index >= numGenerators)
		return NULL;
	return &generators[index];
}



