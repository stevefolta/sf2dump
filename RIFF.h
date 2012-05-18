#ifndef RIFF_h
#define RIFF_h

#include "WinTypes.h"
#include <stdio.h>
#include <string>


struct RIFFChunk {
	enum Type {
		RIFF,
		LIST,
		Custom
		};

	fourcc	id;
	dword 	size;
	Type  	type;
	long  	start;

	void	ReadFrom(FILE* file);
	void	Seek(FILE* file);
	void	SeekAfter(FILE* file);
	long	End() { return start + size; }

	std::string	ReadString(FILE* file);
	};


#endif 	// !RIFF_h

