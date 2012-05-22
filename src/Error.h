#ifndef Error_h
#define Error_h

#include <string>


struct Error {
	std::string	message;

	Error(std::string messageIn)
		: message(messageIn) {}
	};



#endif 	// !Error_h

