#ifndef _UTIL_H_
#define _UTIL_H_
#include <string>
#include <optional>
#include<map>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

std::optional<std::string> file_read(std::string filename);

#endif
