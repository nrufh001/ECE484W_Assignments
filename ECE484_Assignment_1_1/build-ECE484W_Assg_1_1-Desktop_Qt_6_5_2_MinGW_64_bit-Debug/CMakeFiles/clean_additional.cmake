# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ECE484W_Assg_1_1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ECE484W_Assg_1_1_autogen.dir\\ParseCache.txt"
  "ECE484W_Assg_1_1_autogen"
  )
endif()
