#include "SF2.h"


#define SF2Field(type, name) 	\
	fread(&name, sizeof(type), 1, file);


void SF2::iver::ReadFrom(FILE* file)
{
	#include "sf2-chunks/iver.h"
}


void SF2::phdr::ReadFrom(FILE* file)
{
	#include "sf2-chunks/phdr.h"
}



