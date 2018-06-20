#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace FileReader
{
using Params = std::unordered_map<std::string, int>;
using GroupName = std::string;
using Sections = std::unordered_map<GroupName, Params>;

Sections readIniFile(const std::string& fileName);
}