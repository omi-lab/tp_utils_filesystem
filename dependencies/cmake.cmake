
if(WIN32)
  # list(APPEND TP_LIBRARIES "libstdc++fs")
  # list(APPEND TP_LIBRARIES "c++fs")
  # list(APPEND TP_LIBRARIES "libc++fs")
elseif(NOT APPLE)
  list(APPEND TP_LIBRARIES "-lstdc++fs")
endif()
  
list(APPEND TP_DEFINES "TP_FILESYSTEM")
