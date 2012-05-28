#include "RIFF.h"
#include "SF2.h"
#include "SF2Generator.h"
#include "SF2SampleType.h"
#include "Dump.h"
#include "StructuralDump.h"
#include "Indent.h"
#include "Error.h"
#include <stdio.h>
#include <string.h>

using namespace std;


extern void DumpSF2(const char* filename);
extern void DumpPdta(FILE* file, RIFFChunk* pdtaChunk);


int main(int argc, char* argv[])
{
	try {
		bool structural = false;
		for (int whichArg = 1; whichArg < argc; ++whichArg) {
			const char* arg = argv[whichArg];
			if (strcmp(arg, "-s") == 0 ||
			    strcmp(arg, "--structural") == 0 ||
			    strcmp(arg, "-h") == 0) {
				structural = true;
				}
			else {
				if (structural)
					DumpSF2Structurally(arg);
				else
					DumpSF2(arg);
				}
			}
		}
	catch (Error error) {
		fprintf(stderr, "%s\n", error.message.c_str());
		return 1;
		}

	return 0;
}


void DumpSF2(const char* filename)
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
			DumpPdta(file, &chunk);

		indent -= 1;
		chunk.SeekAfter(file);
		}

	fclose(file);
}


void DumpPdta(FILE* file, RIFFChunk* pdtaChunk)
{
	int i, numItems;

	#define StartArray(typeName) 	\
			EmitIndent(); 	\
			printf("'%c%c%c%c':\n", FourCCArgs(chunk.id)); 	\
			indent += 1; 	\
			\
			numItems = chunk.size / SF2::typeName::sizeInFile; 	\
			for (i = 0; i < numItems; ++i) { 	\
				SF2::typeName typeName; 	\
				typeName.ReadFrom(file); 	\
				EmitIndent();
	#define EndArray() 	\
				} 	\
			\
			indent -= 1;

	while (ftell(file) < pdtaChunk->End()) {
		RIFFChunk chunk;
		chunk.ReadFrom(file);

		if (FourCCEquals(chunk.id, "phdr")) {
			StartArray(phdr);
			printf("\"%.20s\":\n", phdr.presetName);
			indent += 1;
			EmitIndent();  printf("preset: %d\n", phdr.preset);
			EmitIndent();  printf("bank: %d\n", phdr.bank);
			EmitIndent();  printf("presetBagNdx: %d\n", phdr.presetBagNdx);
			EmitIndent();  printf("library: %d\n", phdr.library);
			EmitIndent();  printf("genre: %d\n", phdr.genre);
			EmitIndent();  printf("morphology: %d\n", phdr.morphology);
			indent -= 1;
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "pbag")) {
			StartArray(pbag);
			printf("[%d] genNdx: %d  modNdx: %d\n", i, pbag.genNdx, pbag.modNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "pmod")) {
			StartArray(pmod);
			printf(
				"modSrc: %d  modDest: %d  modAmount: %d  "
				"modAmtSrc: %d  modAmtDest: %d\n",
				pmod.modSrcOper, pmod.modDestOper, pmod.modAmount,
				pmod.modAmtSrcOper, pmod.modTransOper);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "pgen")) {
			StartArray(pgen);
			DumpGenerator(i, pgen.genOper, pgen.genAmount);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "inst")) {
			StartArray(inst);
			printf("[%d] \"%s\": %d\n", i, inst.instName, inst.instBagNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "ibag")) {
			StartArray(ibag);
			printf("[%d] instGen: %d  instMod: %d\n", i, ibag.instGenNdx, ibag.instModNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "imod")) {
			StartArray(imod);
			printf(
				"[%d] srcOp: %d  destOp: %d  amount: %d  amtSrcOp: %d  transOp: %d\n",
				imod.modSrcOper, imod.modDestOper, imod.modAmount,
				imod.modAmtSrcOper, imod.modTransOper);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "igen")) {
			StartArray(igen);
			DumpGenerator(i, igen.genOper, igen.genAmount);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "shdr")) {
			StartArray(shdr);
			printf(
				"[%d] \"%.20s\": %lu-%lu  loop: %lu-%lu \n",
				i, shdr.sampleName,
				shdr.start, shdr.end, shdr.startLoop, shdr.endLoop);
			indent += 1;
			EmitIndent();
			const char* sampleType = SampleTypeName(shdr.sampleType);
			char str[64];
			if (sampleType[0] == 0) {
				sprintf(str, "-Illegal (%d)-", shdr.sampleType);
				sampleType = str;
				}
			printf(
				"sampleRate: %lu  origPitch: %d  pitchCorrection: %d  sampleLink: %d  sampleType: %s\n",
				shdr.sampleRate, shdr.originalPitch, shdr.pitchCorrection,
				shdr.sampleLink, sampleType);
			indent -= 1;
			EndArray();
			}
		else
			DumpChunk(&chunk);

		chunk.SeekAfter(file);
		}
}



