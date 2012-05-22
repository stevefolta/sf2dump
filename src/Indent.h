#ifndef Indent_h
#define Indent_h

extern int indent;
extern void EmitIndent();

class Indenter {
	public:
		Indenter() {
			indent += 1;
			}
		~Indenter() {
			indent -= 1;
			}
	};

#endif 	// !Indent_h

