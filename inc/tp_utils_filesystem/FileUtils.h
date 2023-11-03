#ifndef tp_utils_filesystem_FileUtils_h
#define tp_utils_filesystem_FileUtils_h

#include "tp_utils_filesystem/Globals.h"

#include <string>
#include <vector>
#include <unordered_set>

namespace tp_utils_filesystem
{

//##################################################################################################
std::vector<std::string> TP_UTILS_FILESYSTEM_EXPORT listFiles(const std::string& path, const std::unordered_set<std::string>& extensions);

//##################################################################################################
std::vector<std::string> TP_UTILS_FILESYSTEM_EXPORT listDirectories(const std::string& path);

//##################################################################################################
int64_t fileTimeMS(const std::string& path);

//##################################################################################################
bool copyFile(const std::string& pathFrom, const std::string& pathTo);

//##################################################################################################
bool cp(const std::string& pathFrom, const std::string& pathTo, TPRecursive recursive);
    
//##################################################################################################
bool mv(const std::string& pathFrom, const std::string& pathTo);
    
//##################################################################################################
bool mkdir(const std::string& path, TPCreateFullPath createFullPath);

//##################################################################################################
bool rm(const std::string& path, TPRecursive recursive);

//##################################################################################################
bool exists(const std::string& path);

//##################################################################################################
size_t fileSize(const std::string& path);

//##################################################################################################
bool setPermissions(const std::string& path, unsigned permissions);

//##################################################################################################
bool setCWD(const std::string& path);

//##################################################################################################
std::string cwd();

}

#endif
