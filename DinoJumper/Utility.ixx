module;

#include "common.h"

export module Utility;

export class Console
{
public:
	static void Out(std::string text)
	{
		std::cout << text << std::endl;
	};
};
 
