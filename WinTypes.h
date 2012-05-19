#ifndef WinTypes_h
#define WinTypes_h

typedef char fourcc[4];
typedef long dword;
typedef short word;

// Special types for SF2 fields.
typedef char char20[20];


#define FourCCEquals(value1, value2) 	\
	(value1[0] == value2[0] && value1[1] == value2[1] && 	\
	 value1[2] == value2[2] && value1[3] == value2[3])

// Useful in printfs:
#define FourCCArgs(value) 	\
	(value)[0], (value)[1], (value)[2], (value)[3]


#endif 	// !WinTypes_h

