#ifndef _UTILS_
#define _UTILS_

#include <string>
#include <string_view>

bool InitLibraries();
void ReleaseLibraryData();
std::string ReadFile(std::string_view path);

#endif