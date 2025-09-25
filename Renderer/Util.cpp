#include "Util.h"

#include <fstream>
#include <cassert>

// shoutouts terry nguyen real one
std::string aie::DumpToString(const std::string& InputPath)
{
	/* Output to return later */
	std::string OutputString;

	/* Try to open the file (and only use it in this if statement) */
	if (std::ifstream InputFile{ InputPath, std::ios::in | std::ios::ate })
	{
		/* This HAS TO BE TRUE or we CRASH THE PROGRAM (intentionally) */
		assert(InputFile.is_open() && "File did NOT actually open");

		/* Get the current "GET" position to determine how big it is */
		auto Size = InputFile.tellg();
		/* Allocate a string that is suitably large */
		OutputString = std::string(Size, '\0');

		/* Move the "GET" position back to the beginning */
		InputFile.seekg(0);

		/* Write input file to OutputString */
		InputFile.read(&OutputString[0], Size);

		/* Close the file */
		InputFile.close();
	}

	return OutputString;
}

std::string aie::DumpToString(const char* InputPath)
{
	std::string StringInputPath = std::string(InputPath);
	return DumpToString(StringInputPath);
}
