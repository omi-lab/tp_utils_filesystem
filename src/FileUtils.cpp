#include "tp_utils_filesystem/FileUtils.h"

#include "tp_utils/DebugUtils.h"
#include "tp_utils/FileUtils.h"

#ifdef TP_IOS //====================================================================================
//No filesystem support on iOS.

#elif defined(TP_ANDROID) //========================================================================
//No filesystem support on Android.

#elif defined(TP_OSX) //============================================================================
//Still using boost filesystem on mac because filesystem does not appear to be in the std library
//yet.
#define TP_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#define TP_FS

#elif defined(TP_WIN32_MSVC) //=====================================================================
#include <filesystem>
namespace fs = std::filesystem;
#define TP_FS

#elif defined(TP_WIN32_MINGW) //====================================================================
// Seems to be a bug with MinGW / GCC version on Windows.
#define TP_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#define TP_FS

#else //============================================================================================
//On other platforms filesystem is still in experimental.
#include <filesystem>
namespace fs = std::filesystem;
#define TP_FS

#endif

namespace tp_utils_filesystem
{

#ifdef TP_FS
//##################################################################################################
std::vector<std::string> listFiles(const std::string& path, const std::unordered_set<std::string>& extensions)
{
  std::vector<std::string> filenames;

  if(!path.empty())
  {
    try
    {
      for(const fs::path& i : fs::directory_iterator(path))
      {
        if(!fs::is_regular_file(i) && !fs::is_symlink(i))
          continue;

        if(!extensions.empty())
        {
          auto ext = i.extension().string();
          std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
          if(extensions.find('*'+ext) == extensions.end())
            continue;
        }

        filenames.push_back(i.string());
      }
    }
    catch(...)
    {

    }
  }

  return filenames;
}

//##################################################################################################
std::vector<std::string> listDirectories(const std::string& path)
{
  std::vector<std::string> filenames;

  if(!path.empty())
  {
    try
    {
      for(const fs::path& i : fs::directory_iterator(path))
      {
        if(!fs::is_directory(i))
          continue;

        filenames.push_back(i.string());
      }
    }
    catch(...)
    {

    }
  }

  return filenames;
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
      fs::copy_file(pathFrom, pathTo, fs::copy_options::overwrite_existing, ec);
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
bool cp(const std::string& pathFrom, const std::string& pathTo, TPRecursive recursive)
{
  if(!pathFrom.empty() && !pathTo.empty())
  {
    try
    {
#ifdef TP_BOOST_FILESYSTEM
      boost::system::error_code ec;
      fs::copy_options options = fs::copy_options::overwrite_existing;
      if(recursive == TPRecursive::Yes)
        options |= fs::copy_options::recursive;

      fs::copy(pathFrom, pathTo, options, ec);
      return !bool(ec);
#else
      fs::copy_options options = fs::copy_options::overwrite_existing;
      if(recursive == TPRecursive::Yes)
        options |= fs::copy_options::recursive;

      fs::copy(pathFrom, pathTo, options);
      return true;
#endif
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool mv(const std::string& pathFrom, const std::string& pathTo)
{
  if(!pathFrom.empty() && !pathTo.empty())
  {
    try
    {
#ifdef TP_BOOST_FILESYSTEM
      boost::system::error_code ec;
      fs::rename(pathFrom, pathTo, ec);
      return !bool(ec);
#else
      fs::rename(pathFrom, pathTo);
      return true;
#endif
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
bool mkdir(const std::string& path, TPCreateFullPath createFullPath)
{
  if(!path.empty())
  {
    try
    {
      if(createFullPath == TPCreateFullPath::Yes)
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
bool rm(const std::string& path, TPRecursive recursive)
{
  bool ok=false;

  if(!path.empty())
  {    
#ifdef TP_BOOST_FILESYSTEM
    boost::system::error_code ec;

    if(recursive == TPRecursive::Yes)
      ok = fs::remove_all(path, ec);
    else
      ok = fs::remove(path, ec);

    if(ec.failed())
    {
      tpWarning() << "Failed to rm: " << path << " error (" << ec.message() << ')';
      return false;
    }
#else
    try
    {
      if(recursive == TPRecursive::Yes)
        ok = fs::remove_all(path);
      else
        ok = fs::remove(path);
    }
    catch (...)
    {

    }
#endif
  }

  return ok;
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

//##################################################################################################
bool setPermissions(const std::string& path, unsigned permissions)
{
  unsigned perms=0;

  perms |= (permissions & tp_utils::owner_read  )?unsigned(fs::perms::owner_read  ):0;
  perms |= (permissions & tp_utils::owner_write )?unsigned(fs::perms::owner_write ):0;
  perms |= (permissions & tp_utils::owner_all   )?unsigned(fs::perms::owner_all   ):0;

  perms |= (permissions & tp_utils::group_read  )?unsigned(fs::perms::group_read  ):0;
  perms |= (permissions & tp_utils::group_write )?unsigned(fs::perms::group_write ):0;
  perms |= (permissions & tp_utils::group_all   )?unsigned(fs::perms::group_all   ):0;

  perms |= (permissions & tp_utils::others_read )?unsigned(fs::perms::others_read ):0;
  perms |= (permissions & tp_utils::others_write)?unsigned(fs::perms::others_write):0;
  perms |= (permissions & tp_utils::others_all  )?unsigned(fs::perms::others_all  ):0;

#ifdef TP_BOOST_FILESYSTEM
  perms |= (permissions & tp_utils::owner_exec  )?unsigned(fs::perms::owner_exe   ):0;
  perms |= (permissions & tp_utils::group_exec  )?unsigned(fs::perms::group_exe   ):0;
  perms |= (permissions & tp_utils::others_exec )?unsigned(fs::perms::others_exe  ):0;
  try
  {
    fs::permissions(path, fs::add_perms|fs::perms(perms));
    fs::permissions(path, fs::remove_perms|~fs::perms(perms));
    return true;
  }
  catch(...)
  {
  }
#else
  perms |= (permissions & tp_utils::owner_exec  )?unsigned(fs::perms::owner_exec ):0;
  perms |= (permissions & tp_utils::group_exec  )?unsigned(fs::perms::group_exec ):0;
  perms |= (permissions & tp_utils::others_exec )?unsigned(fs::perms::others_exec):0;

  std::error_code ec;

#ifdef TP_WIN32_MSVC
  fs::permissions(path, fs::perms(perms), fs::perm_options::add, ec);
  fs::permissions(path, ~fs::perms(perms), fs::perm_options::remove, ec);
#elif defined(TP_LINUX)
  fs::permissions(path, fs::perms(perms), fs::perm_options::add, ec);
  fs::permissions(path, ~fs::perms(perms), fs::perm_options::remove, ec);
#else
  fs::permissions(path, fs::perms::add_perms|fs::perms(perms), ec);
  fs::permissions(path, fs::perms::remove_perms|~fs::perms(perms), ec);
#endif

#endif

  return false;
}

//##################################################################################################
bool setCWD(const std::string& path)
{
  if(!path.empty())
  {
    try
    {
      fs::current_path(path);
      return true;
    }
    catch(...)
    {

    }
  }

  return false;
}

//##################################################################################################
std::string cwd()
{
  try
  {
#ifdef TP_WIN32_MSVC
    std::wstring s = fs::current_path().c_str();
    return tpToUTF8(s);
#else
    return fs::current_path().c_str();
#endif
  }
  catch(...)
  {

  }
  return std::string();
}

#else

//##################################################################################################
std::vector<std::string> listFiles(const std::string& path, const std::unordered_set<std::string>& extensions)
{
  std::vector<std::string> filenames;
  TP_UNUSED(path);
  TP_UNUSED(extensions);
  return filenames;
}

//##################################################################################################
std::vector<std::string> listDirectories(const std::string& path)
{
  std::vector<std::string> filenames;
  TP_UNUSED(path);
  return filenames;
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
bool cp(const std::string& pathFrom, const std::string& pathTo, bool recursive)
{
  TP_UNUSED(pathFrom);
  TP_UNUSED(pathTo);
  return false;
}

//##################################################################################################
bool mv(const std::string& pathFrom, const std::string& pathTo)
{
  TP_UNUSED(pathFrom);
  TP_UNUSED(pathTo);
  return false;
}

//##################################################################################################
bool mkdir(const std::string& path, TPCreateFullPath createFullPath)
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
