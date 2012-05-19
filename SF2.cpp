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


void SF2::pbag::ReadFrom(FILE* file)
{
	#include "sf2-chunks/pbag.h"
}


void SF2::pmod::ReadFrom(FILE* file)
{
	#include "sf2-chunks/pmod.h"
}


void SF2::pgen::ReadFrom(FILE* file)
{
	#include "sf2-chunks/pgen.h"
}


void SF2::inst::ReadFrom(FILE* file)
{
	#include "sf2-chunks/inst.h"
}


void SF2::ibag::ReadFrom(FILE* file)
{
	#include "sf2-chunks/ibag.h"
}


void SF2::imod::ReadFrom(FILE* file)
{
	#include "sf2-chunks/imod.h"
}


void SF2::igen::ReadFrom(FILE* file)
{
	#include "sf2-chunks/igen.h"
}


void SF2::shdr::ReadFrom(FILE* file)
{
	#include "sf2-chunks/shdr.h"
}



