#pragma once

#include <string>

namespace aie
{
	/// <summary>
	/// Returns a string with the contents of the given file.
	/// </summary>
	/// <param name="InputPath">The path to the file</param>
	/// <returns>The contents.</returns>
	std::string DumpToString(const std::string& InputPath);

	std::string DumpToString(const char* InputPath);
}