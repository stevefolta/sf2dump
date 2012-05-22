#include "RIFF.h"
#include "SF2.h"
#include "SF2Generator.h"
#include "Indent.h"
#include "Error.h"
#include <stdio.h>

using namespace std;


extern void DumpSF2(const char* filename);
extern void DumpINFO(FILE* file, RIFFChunk* infoChunk);
extern void DumpPdta(FILE* file, RIFFChunk* pdtaChunk);
extern void DumpGenerator(int index, word genOper, SF2::genAmountType genAmount);
extern void DumpChunk(RIFFChunk* chunk);


int main(int argc, char* argv[])
{
	try {
		for (int whichArg = 1; whichArg < argc; ++whichArg)
			DumpSF2(argv[whichArg]);
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


void DumpINFO(FILE* file, RIFFChunk* infoChunk)
{
	while (ftell(file) < infoChunk->End()) {
		RIFFChunk chunk;
		chunk.ReadFrom(file);

		if (FourCCEquals(chunk.id, "ifil") || FourCCEquals(chunk.id, "iver")) {
			SF2::iver iver;
			iver.ReadFrom(file);
			EmitIndent();
			printf("'%c%c%c%c': %d.%d\n", FourCCArgs(chunk.id), iver.major, iver.minor);
			}
		else {
			static const char* stringChunks[] = {
				"isng", "INAM", "ICRD", "IENG", "IPRD", "ICOP", "ICMT", "ISFT",
				""
				};
			bool isStringChunk = false;
			for (int i = 0; ; ++i) {
				if (stringChunks[i][0] == 0)
					break;
				if (FourCCEquals(chunk.id, stringChunks[i])) {
					isStringChunk = true;
					break;
					}
				}
			if (isStringChunk) {
				EmitIndent();
				printf("'%c%c%c%c': \"%s\"\n", FourCCArgs(chunk.id), chunk.ReadString(file).c_str());
				}
			else
				DumpChunk(&chunk);
			}

		chunk.SeekAfter(file);
		}
}


void DumpPdta(FILE* file, RIFFChunk* pdtaChunk)
{
	int i, numItems;

	#define StartArray(typeName, fileSize) 	\
			EmitIndent(); 	\
			printf("'%c%c%c%c':\n", FourCCArgs(chunk.id)); 	\
			indent += 1; 	\
			\
			numItems = chunk.size / fileSize; 	\
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
			StartArray(phdr, 38);
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
			StartArray(pbag, 4);
			printf("[%d] genNdx: %d  modNdx: %d\n", i, pbag.genNdx, pbag.modNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "pmod")) {
			StartArray(pmod, 10);
			printf(
				"modSrc: %d  modDest: %d  modAmount: %d  "
				"modAmtSrc: %d  modAmtDest: %d\n",
				pmod.modSrcOper, pmod.modDestOper, pmod.modAmount,
				pmod.modAmtSrcOper, pmod.modTransOper);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "pgen")) {
			StartArray(pgen, 4);
			DumpGenerator(i, pgen.genOper, pgen.genAmount);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "inst")) {
			StartArray(inst, 22);
			printf("[%d] \"%s\": %d\n", i, inst.instName, inst.instBagNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "ibag")) {
			StartArray(ibag, 4);
			printf("[%d] instGen: %d  instMod: %d\n", i, ibag.instGenNdx, ibag.instModNdx);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "imod")) {
			StartArray(imod, 10);
			printf(
				"[%d] srcOp: %d  destOp: %d  amount: %d  amtSrcOp: %d  transOp: %d\n",
				imod.modSrcOper, imod.modDestOper, imod.modAmount,
				imod.modAmtSrcOper, imod.modTransOper);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "igen")) {
			StartArray(igen, 4);
			DumpGenerator(i, igen.genOper, igen.genAmount);
			EndArray();
			}
		else if (FourCCEquals(chunk.id, "shdr")) {
			StartArray(shdr, 46);
			printf(
				"[%d] \"%s\": %lu-%lu  loop: %lu-%lu \n",
				i, shdr.sampleName,
				shdr.start, shdr.end, shdr.startLoop, shdr.endLoop);
			indent += 1;
			EmitIndent();
			printf(
				"sampleRate: %lu  origPitch: %d  pitchCorrection: %d  sampleLink: %d  sampleType: %d\n",
				shdr.sampleRate, shdr.originalPitch, shdr.pitchCorrection,
				shdr.sampleLink, shdr.sampleType);
			indent -= 1;
			EndArray();
			}
		else
			DumpChunk(&chunk);

		chunk.SeekAfter(file);
		}
}


void DumpGenerator(int index, word genOper, SF2::genAmountType genAmount)
{
	const SF2Generator* generator = GeneratorFor(genOper);
	if (generator == NULL) {
		printf(
			"[%d] genOper: %d  genAmount: %d/%d/%d-%d\n",
			index, genOper,
			genAmount.wordAmount, genAmount.shortAmount,
			genAmount.ranges.lo, genAmount.ranges.hi);
		}
	else {
		switch (generator->type) {
			case SF2Generator::Short:
				printf(
					"[%d] %s (%d): %d\n",
					index, generator->name, genOper, genAmount.shortAmount);
				break;
			case SF2Generator::Word:
				printf(
					"[%d] %s (%d): %d\n",
					index, generator->name, genOper, genAmount.wordAmount);
				break;
			case SF2Generator::Range:
				printf(
					"[%d] %s (%d): %d-%d\n",
					index, generator->name, genOper,
					genAmount.ranges.hi, genAmount.ranges.lo);
				break;
			}
		}
}


void DumpChunk(RIFFChunk* chunk)
{
	EmitIndent();
	printf("'%c%c%c%c' size %ld.\n", FourCCArgs(chunk->id), chunk->size);
}



