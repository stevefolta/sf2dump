#include "StructuralDump.h"
#include "RIFF.h"
#include "SF2.h"
#include "SF2Generator.h"
#include "Dump.h"
#include "Indent.h"
#include "Error.h"
#include <stdio.h>
#include <string>

using namespace std;

extern void DumpPdtaStructurally(FILE* file, RIFFChunk* pdtaChunk);


struct Hydra {
	SF2::phdr*	phdr;
	SF2::pbag*	pbag;
	SF2::pmod*	pmod;
	SF2::pgen*	pgen;
	SF2::inst*	inst;
	SF2::ibag*	ibag;
	SF2::imod*	imod;
	SF2::igen*	igen;
	SF2::shdr*	shdr;

	int	phdrNumItems, pbagNumItems, pmodNumItems, pgenNumItems;
	int	instNumItems, ibagNumItems, imodNumItems, igenNumItems;
	int	shdrNumItems;

	Hydra();
	~Hydra();
	};

void DumpSF2Structurally(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		string message = "Couldn't open \"";
		message += filename;
		message += "\".";
		throw Error(message);
		return;
		}

	RIFFChunk riffChunk;
	riffChunk.ReadFrom(file);
	DumpChunk(&riffChunk);
	indent += 1;

	while (ftell(file) < riffChunk.End()) {
		RIFFChunk chunk;
		chunk.ReadFrom(file);
		DumpChunk(&chunk);
		indent += 1;

		if (FourCCEquals(chunk.id, "INFO"))
			DumpINFO(file, &chunk);
		else if (FourCCEquals(chunk.id, "pdta"))
			DumpPdtaStructurally(file, &chunk);

		indent -= 1;
		chunk.SeekAfter(file);
		}

	fclose(file);
}


void DumpPdtaStructurally(FILE* file, RIFFChunk* pdtaChunk)
{
	// Read the hydra.
	int i, numItems;
	Hydra hydra;

	#define HandleChunk(chunkName) 	\
		if (FourCCEquals(chunk.id, #chunkName)) { 	\
			numItems = chunk.size / SF2::chunkName::sizeInFile; 	\
			hydra.chunkName##NumItems = numItems; 	\
			hydra.chunkName = new SF2::chunkName[numItems]; 	\
			for (i = 0; i < numItems; ++i) 	\
				hydra.chunkName[i].ReadFrom(file); 	\
			} 	\
		else

	while (ftell(file) < pdtaChunk->End()) {
		RIFFChunk chunk;
		chunk.ReadFrom(file);

		HandleChunk(phdr)
		HandleChunk(pbag)
		HandleChunk(pmod)
		HandleChunk(pgen)
		HandleChunk(inst)
		HandleChunk(ibag)
		HandleChunk(imod)
		HandleChunk(igen)
		HandleChunk(shdr)
		{}

		chunk.SeekAfter(file);
		}

	// Dump.
	EmitIndent();  printf("Presets:\n");
	indent += 1;
	for (int whichPreset = 0; whichPreset < hydra.phdrNumItems; ++whichPreset) {
		SF2::phdr* phdr = &hydra.phdr[whichPreset];
		EmitIndent();  printf("\"%.20s\":\n", phdr->presetName);
		indent += 1;
		EmitIndent();  printf("preset: %d\n", phdr->preset);
		EmitIndent();  printf("bank: %d\n", phdr->bank);
		EmitIndent();  printf("presetBagNdx: %d\n", phdr->presetBagNdx);
		EmitIndent();  printf("library: %d\n", phdr->library);
		EmitIndent();  printf("genre: %d\n", phdr->genre);
		EmitIndent();  printf("morphology: %d\n", phdr->morphology);
		if (whichPreset < hydra.phdrNumItems - 1) {
			EmitIndent();  printf("Zones:\n");
			Indenter indenter;
			int zoneEnd = phdr[1].presetBagNdx;
			for (int whichZone = phdr->presetBagNdx; whichZone < zoneEnd; ++whichZone) {
				SF2::pbag* pbag = &hydra.pbag[whichZone];
				EmitIndent();	printf("[%d]:\n", whichZone);
				}
			}
		indent -= 1;
		}
	indent -= 1;
}


Hydra::Hydra()
: phdr(NULL), pbag(NULL), pmod(NULL), pgen(NULL),
	inst(NULL), ibag(NULL), imod(NULL), igen(NULL),
	shdr(NULL)
{
}


Hydra::~Hydra()
{
	delete phdr;
	delete pbag;
	delete pmod;
	delete pgen;
	delete inst;
	delete ibag;
	delete imod;
	delete igen;
	delete shdr;
}



