#include "SF2SampleType.h"

const char* SampleTypeName(int sampleType)
{
	switch (sampleType) {
		case 1:
			return "monoSample";
		case 2:
			return "rightSample";
		case 4:
			return "leftSample";
		case 8:
			return "linkedSample";
		case 32769:
			return "RomMonoSample";
		case 32770:
			return "RomRightSample";
		case 32772:
			return "RomLeftSample";
		case 32776:
			return "RomLinkedSample";
		}

	return "";
}



