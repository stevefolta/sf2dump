#ifndef SF2_h
#define SF2_h

#include "WinTypes.h"
#include <stdio.h>

#define SF2Field(type, name) 	\
	type	name;

namespace SF2 {

	struct iver {
		#include "sf2-chunks/iver.h"
		void	ReadFrom(FILE* file);
		};

	struct phdr {
		#include "sf2-chunks/phdr.h"
		void	ReadFrom(FILE* file);
		};
	};

#undef SF2Field


#endif 	// !SF2_h

