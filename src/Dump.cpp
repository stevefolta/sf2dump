#include "Dump.h"
#include "RIFF.h"
#include "SF2.h"
#include "SF2Generator.h"
#include "Indent.h"
#include "Error.h"
#include <stdio.h>


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
					genAmount.ranges.lo, genAmount.ranges.hi);
				break;
			}
		}
}


void DumpChunk(RIFFChunk* chunk)
{
	EmitIndent();
	printf("'%c%c%c%c' size %ld at 0x%08X.\n", FourCCArgs(chunk->id), chunk->size, chunk->start);
}


