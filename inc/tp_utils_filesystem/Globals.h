#ifndef tp_utils_filesystem_Globals_h
#define tp_utils_filesystem_Globals_h

#include "tp_utils/Globals.h"

#if defined(TP_UTILS_FILESYSTEM_LIBRARY)
#  define TP_UTILS_FILESYSTEM_SHARED_EXPORT TP_EXPORT
#else
#  define TP_UTILS_FILESYSTEM_SHARED_EXPORT TP_IMPORT
#endif

//##################################################################################################
//! Filesystem wrapper.
namespace tp_utils_filesystem
{

//##################################################################################################
//! Call this once at start of main
void init();

}

#endif
