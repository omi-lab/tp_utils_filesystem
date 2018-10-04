#include "tp_utils_filesystem/Globals.h"
#include "tp_utils_filesystem/FileUtils.h"

#include "tp_utils/FileUtils.h"

namespace tp_utils_filesystem
{

//##################################################################################################
void init()
{
  tp_utils::listFilesCallback = listFiles;
  tp_utils::listDirectoriesCallback = listDirectories;
  tp_utils::fileTimeMSCallback = fileTimeMS;
  tp_utils::copyFileCallback = copyFile;
  tp_utils::mkdirCallback = mkdir;
}

}
