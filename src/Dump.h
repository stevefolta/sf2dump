#ifndef Dump_h
#define Dump_h

#include "RIFF.h"
#include "SF2.h"
#include <stdio.h>


extern void DumpINFO(FILE* file, RIFFChunk* infoChunk);
extern void DumpGenerator(int index, word genOper, SF2::genAmountType genAmount);
extern void DumpChunk(RIFFChunk* chunk);

#endif 	// !Dump_h

