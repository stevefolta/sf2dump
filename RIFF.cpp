#include "RIFF.h"


void RIFFChunk::ReadFrom(FILE* file)
{
	fread(&id, sizeof(fourcc), 1, file);
	fread(&size, sizeof(dword), 1, file);
	start = ftell(file);

	if (FourCCEquals(id, "RIFF")) {
		type = RIFF;
		fread(&id, sizeof(fourcc), 1, file);
		start += sizeof(fourcc);
		size -= sizeof(fourcc);
		}
	else if (FourCCEquals(id, "LIST")) {
		type = LIST;
		fread(&id, sizeof(fourcc), 1, file);
		start += sizeof(fourcc);
		size -= sizeof(fourcc);
		}
	else
		type = Custom;
}


void RIFFChunk::Seek(FILE* file)
{
	fseek(file, start, SEEK_SET);
}


void RIFFChunk::SeekAfter(FILE* file)
{
	long next = start + size;
	if (next % 2 != 0)
		next += 1;
	fseek(file, next, SEEK_SET);
}


std::string RIFFChunk::ReadString(FILE* file)
{
	char str[size];
	fread(str, size, 1, file);
	return std::string(str);
}




