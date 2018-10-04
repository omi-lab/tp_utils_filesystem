#ifndef tp_utils_filesystem_FileUtils_h
#define tp_utils_filesystem_FileUtils_h

#include "tp_utils_filesystem/Globals.h"

#include <string>
#include <vector>
#include <unordered_set>

namespace tp_utils_filesystem
{

//##################################################################################################
std::vector<std::string> TP_UTILS_FILESYSTEM_SHARED_EXPORT listFiles(const std::string& path, const std::unordered_set<std::string>& extensions);

//##################################################################################################
std::vector<std::string> TP_UTILS_FILESYSTEM_SHARED_EXPORT listDirectories(const std::string& path);

//##################################################################################################
int64_t fileTimeMS(const std::string& path);

//##################################################################################################
bool copyFile(const std::string& pathFrom, const std::string& pathTo);

//##################################################################################################
bool mkdir(const std::string& path, bool createFullPath);
}

#endif
