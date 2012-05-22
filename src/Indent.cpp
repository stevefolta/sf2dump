#include "Indent.h"
#include <stdio.h>


int indent = 0;


void EmitIndent()
{
	for (int i = 0; i < indent; ++i)
		printf("  ");
}



