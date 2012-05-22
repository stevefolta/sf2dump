#ifndef SF2_h
#define SF2_h

#include "WinTypes.h"
#include <stdio.h>

#define SF2Field(type, name) 	\
	type	name;

namespace SF2 {

	struct rangesType {
		byte	lo, hi;
		};

	union genAmountType {
		rangesType	ranges;
		short     	shortAmount;
		word      	wordAmount;
		};

	struct iver {
		#include "sf2-chunks/iver.h"
		void	ReadFrom(FILE* file);
		};

	struct phdr {
		#include "sf2-chunks/phdr.h"
		void	ReadFrom(FILE* file);
		static const int sizeInFile = 38;
		};

	struct pbag {
		#include "sf2-chunks/pbag.h"
		void	ReadFrom(FILE* file);
		static const int sizeInFile = 4;
		};

	struct pmod {
		#include "sf2-chunks/pmod.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 10;
		};

	struct pgen {
		#include "sf2-chunks/pgen.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 4;
		};

	struct inst {
		#include "sf2-chunks/inst.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 22;
		};

	struct ibag {
		#include "sf2-chunks/ibag.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 4;
		};

	struct imod {
		#include "sf2-chunks/imod.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 10;
		};

	struct igen {
		#include "sf2-chunks/igen.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 4;
		};

	struct shdr {
		#include "sf2-chunks/shdr.h"
		void	ReadFrom(FILE* file);
		static const int	sizeInFile = 46;
		};

	};

#undef SF2Field


#endif 	// !SF2_h

