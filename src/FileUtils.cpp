#include "tp_utils_filesystem/FileUtils.h"


#ifdef TP_IOS //===================================================================================
//No filesystem support on iOS.

#elif defined(TP_ANDROID) //=======================================================================
//No filesystem support on Android.

#elif defined(TP_OSX) //===========================================================================
//Still using boost filesystem on mac because filesystem does not appear to be in the std library
//yet.
#define TP_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#define TP_FS

#else //============================================================================================
//On other platforms filesystem is still in experimental.
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#define TP_FS

#endif

namespace tp_utils_filesystem
{

#ifdef TP_FS
//##################################################################################################
std::vector<std::string> listFiles(const std::string& path, const std::unordered_set<std::string>& extensions)
{
  std::vector<std::string> fileNames;

  if(!path.empty())
  {
    try
    {
      for(const fs::path& i : fs::directory_iterator(path))
      {
        if(!fs::is_regular_file(i))
          continue;

        auto ext = i.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if(extensions.find('*'+ext) == extensions.end())
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
      for(const fs::path& i : fs::directory_iterator(path))
      {
        if(!fs::is_directory(i))
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
#ifdef TP_BOOST_FILESYSTEM
      timestamp=int64_t(fs::last_write_time(path))*1000ll;
#else
      timestamp=std::chrono::duration_cast<std::chrono::milliseconds>(fs::last_write_time(path).time_since_epoch()).count();
#endif
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
#ifdef TP_BOOST_FILESYSTEM
      boost::system::error_code ec;
      fs::copy_file(pathFrom, pathTo, fs::copy_option::overwrite_if_exists, ec);
      return !bool(ec);
#else
      return fs::copy_file(pathFrom, pathTo, fs::copy_options::overwrite_existing);
#endif
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool mkdir(const std::string& path, tp_utils::CreateFullPath createFullPath)
{
  if(!path.empty())
  {
    try
    {
      if(createFullPath == tp_utils::CreateFullPath::Yes)
        return fs::create_directories(path);
      return fs::create_directory(path);
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
        return fs::remove_all(path);
      return fs::remove(path);
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool exists(const std::string& path)
{
  if(!path.empty())
  {
    try
    {
      return fs::exists(path);
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
size_t fileSize(const std::string& path)
{

  if(!path.empty())
  {
    try
    {
      return size_t(fs::file_size(path));
    }
    catch(...)
    {

    }
  }

  return 0;
}

#else

//##################################################################################################
std::vector<std::string> listFiles(const std::string& path, const std::unordered_set<std::string>& extensions)
{
  std::vector<std::string> fileNames;
  TP_UNUSED(path);
  TP_UNUSED(extensions);
  return fileNames;
}

//##################################################################################################
std::vector<std::string> listDirectories(const std::string& path)
{
  std::vector<std::string> fileNames;
  TP_UNUSED(path);
  return fileNames;
}

//##################################################################################################
int64_t fileTimeMS(const std::string& path)
{
  TP_UNUSED(path);
  return 0;
}

//##################################################################################################
bool copyFile(const std::string& pathFrom, const std::string& pathTo)
{
  TP_UNUSED(pathFrom);
  TP_UNUSED(pathTo);
  return false;
}

//##################################################################################################
bool mkdir(const std::string& path, tp_utils::CreateFullPath createFullPath)
{
  TP_UNUSED(path);
  TP_UNUSED(createFullPath);
  return false;
}

//##################################################################################################
bool rm(const std::string& path, bool recursive)
{
  TP_UNUSED(path);
  TP_UNUSED(recursive);
  return false;
}

//##################################################################################################
bool exists(const std::string& path)
{
  TP_UNUSED(path);
  return false;
}

#endif

}
