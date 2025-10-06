# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\projetc_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\projetc_autogen.dir\\ParseCache.txt"
  "projetc_autogen"
  )
endif()
