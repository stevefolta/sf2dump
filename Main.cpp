#include "RIFF.h"
#include "SF2.h"
#include <stdio.h>
#include <string>

using namespace std;


struct Error {
	string	message;

	Error(string messageIn)
		: message(messageIn) {}
	};


extern void DumpSF2(const char* filename);
extern void DumpINFO(FILE* file, RIFFChunk* infoChunk);
extern void DumpChunk(RIFFChunk* chunk);

static int indent = 0;
void EmitIndent()
{
	for (int i = 0; i < indent; ++i)
		printf("  ");
}


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
			fread(&iver, sizeof(iver), 1, file);
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


void DumpChunk(RIFFChunk* chunk)
{
	EmitIndent();
	printf("'%c%c%c%c' size %ld.\n", FourCCArgs(chunk->id), chunk->size);
}



