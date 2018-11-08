#include "tp_utils_filesystem/FileUtils.h"

#include <experimental/filesystem>

namespace tp_utils_filesystem
{

//##################################################################################################
std::vector<std::string> listFiles(const std::string& path, const std::unordered_set<std::string>& extensions)
{
  std::vector<std::string> fileNames;

  if(!path.empty())
  {
    try
    {
      for(const std::experimental::filesystem::path& i : std::experimental::filesystem::directory_iterator(path))
      {
        if(!std::experimental::filesystem::is_regular_file(i))
          continue;

        if(extensions.find('*'+i.extension().string()) == extensions.end())
          continue;

        fileNames.push_back(i.string());
      }
    }
    catch(...)
    {

    }
  }

  return fileNames;
}

//##################################################################################################
std::vector<std::string> listDirectories(const std::string& path)
{
  std::vector<std::string> fileNames;

  if(!path.empty())
  {
    try
    {
      for(const std::experimental::filesystem::path& i : std::experimental::filesystem::directory_iterator(path))
      {
        if(!std::experimental::filesystem::is_directory(i))
          continue;

        fileNames.push_back(i.string());
      }
    }
    catch(...)
    {

    }
  }

  return fileNames;
}

//##################################################################################################
int64_t fileTimeMS(const std::string& path)
{
  int64_t timestamp=0;
  if(!path.empty())
  {
    try
    {
      timestamp=std::chrono::duration_cast<std::chrono::milliseconds>(std::experimental::filesystem::last_write_time(path).time_since_epoch()).count();
    }
    catch(...)
    {

    }
  }

  return timestamp;
}

//##################################################################################################
bool copyFile(const std::string& pathFrom, const std::string& pathTo)
{
  if(!pathFrom.empty() && !pathTo.empty())
  {
    try
    {
      return std::experimental::filesystem::copy_file(pathFrom, pathTo);
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool mkdir(const std::string& path, bool createFullPath)
{
  if(!path.empty())
  {
    try
    {
      if(createFullPath)
        return std::experimental::filesystem::create_directories(path);
      else
        return std::experimental::filesystem::create_directory(path);
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool rm(const std::string& path, bool recursive)
{
  if(!path.empty())
  {
    try
    {
      if(recursive)
        return std::experimental::filesystem::remove_all(path);
      else
        return std::experimental::filesystem::remove(path);
    }
    catch(...)
    {

    }
  }

  return false;
}

}
