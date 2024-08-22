
if(WIN32)
  # list(APPEND TP_LIBRARIES "libstdc++fs")
  # list(APPEND TP_LIBRARIES "c++fs")
  # list(APPEND TP_LIBRARIES "libc++fs")
  list(APPEND TP_DEFINES "TP_FILESYSTEM")
else()
  list(APPEND TP_LIBRARIES "-lstdc++fs")
  list(APPEND TP_DEFINES "TP_FILESYSTEM")
endif()

