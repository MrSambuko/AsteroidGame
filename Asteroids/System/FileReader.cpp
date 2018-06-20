#include <fstream>
#include <regex>

#include "FileReader.hpp"

static const std::regex groupNameRegex(R"(\[(\w+)\])");
static const std::regex paramRegex(R"((\w+)\s*=\s*(\d+))");

namespace FileReader
{

Sections readIniFile( const std::string& fileName )
{
	std::fstream file;
	file.open(fileName, std::fstream::in);
	
	std::smatch match;
	std::string line;

	Sections result;

	while (std::getline(file, line))
	{
		if (std::regex_search(line, match, groupNameRegex))
		{
			const GroupName& groupName = match[1];

			Params params;

			while (std::getline(file, line) && std::regex_search(line, match, paramRegex))
				params[match[1]] = std::stoi(match[2]);

			result[groupName] = params;
		}
	}

	return std::move(result);
}

}