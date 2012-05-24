#include "StructuralDump.h"
#include "RIFF.h"
#include "SF2.h"
#include "SF2Generator.h"
#include "SF2SampleType.h"
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

	// Dump presets.
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
				Indenter indenter;
				int genEnd = pbag[1].genNdx;
				int whichGen = pbag->genNdx;
				if (whichGen < genEnd) {
					EmitIndent();  printf("Generators:\n");
					Indenter indenter;
					for (; whichGen < genEnd; ++whichGen) {
						SF2::pgen* pgen = &hydra.pgen[whichGen];
						EmitIndent();  DumpGenerator(whichGen, pgen->genOper, pgen->genAmount);
						}
					}
				int modEnd = pbag[1].modNdx;
				int whichMod = pbag->modNdx;
				if (whichMod < modEnd) {
					EmitIndent();  printf("Modulators:\n");
					Indenter indenter;
					for (; whichMod < modEnd; ++whichMod) {
						SF2::pmod* pmod = &hydra.pmod[whichMod];
						EmitIndent();
						printf(
							"[%d] "
							"modSrc: %d  modDest: %d  modAmount: %d  "
							"modAmtSrc: %d  modAmtDest: %d\n",
							whichMod,
							pmod->modSrcOper, pmod->modDestOper, pmod->modAmount,
							pmod->modAmtSrcOper, pmod->modTransOper);
						}
					}
				}
			}
		indent -= 1;
		}
	indent -= 1;
	printf("\n");

	// Dump instruments.
	EmitIndent();  printf("Instruments:\n");
	indent += 1;
	for (int whichInst = 0; whichInst < hydra.instNumItems; ++whichInst) {
		SF2::inst* inst = &hydra.inst[whichInst];
		bool isLastInst = (whichInst >= hydra.instNumItems - 1);
		EmitIndent();
		printf(
			"[%d] \"%.20s\"%s\n",
			whichInst, inst->instName, (isLastInst ? "" : ":"));
		Indenter indenter;
		if (!isLastInst) {
			EmitIndent();  printf("Zones:\n");
			Indenter indenter;
			int zoneEnd = inst[1].instBagNdx;
			for (int whichZone = inst->instBagNdx; whichZone < zoneEnd; ++whichZone) {
				SF2::ibag* ibag = &hydra.ibag[whichZone];
				EmitIndent();	printf("[%d]:\n", whichZone);
				Indenter indenter;
				int genEnd = ibag[1].instGenNdx;
				int whichGen = ibag->instGenNdx;
				if (whichGen < genEnd) {
					EmitIndent();  printf("Generators:\n");
					Indenter indenter;
					for (; whichGen < genEnd; ++whichGen) {
						SF2::igen* igen = &hydra.igen[whichGen];
						EmitIndent();  DumpGenerator(whichGen, igen->genOper, igen->genAmount);
						if (igen->genOper == SF2Generator::sampleID) {
							Indenter indenter;
							int whichSample = igen->genAmount.wordAmount;
							SF2::shdr* shdr = &hydra.shdr[whichSample];
							EmitIndent();
							printf(
								"[%d] \"%s\": %lu-%lu  loop: %lu-%lu \n",
								whichSample, shdr->sampleName,
								shdr->start, shdr->end, shdr->startLoop, shdr->endLoop);
							indent += 1;
							EmitIndent();
							const char* sampleType = SampleTypeName(shdr->sampleType);
							char str[64];
							if (sampleType[0] == 0) {
								sprintf(str, "-Illegal (%d)-", shdr->sampleType);
								sampleType = str;
								}
							printf(
								"sampleRate: %lu  origPitch: %d  pitchCorrection: %d  "
								"sampleLink: %d  sampleType: %s\n",
								shdr->sampleRate, shdr->originalPitch, shdr->pitchCorrection,
								shdr->sampleLink, sampleType);
							indent -= 1;
							}
						}
					}
				int modEnd = ibag[1].instModNdx;
				int whichMod = ibag->instModNdx;
				if (whichMod < modEnd) {
					EmitIndent();  printf("Modulators:\n");
					Indenter indenter;
					for (; whichMod < modEnd; ++whichMod) {
						SF2::imod* imod = &hydra.imod[whichMod];
						EmitIndent();
						printf(
							"[%d] "
							"modSrc: %d  modDest: %d  modAmount: %d  "
							"modAmtSrc: %d  modAmtDest: %d\n",
							whichMod,
							imod->modSrcOper, imod->modDestOper, imod->modAmount,
							imod->modAmtSrcOper, imod->modTransOper);
						}
					}
				}
			}
		}
	indent -= 1;
	printf("\n");
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



