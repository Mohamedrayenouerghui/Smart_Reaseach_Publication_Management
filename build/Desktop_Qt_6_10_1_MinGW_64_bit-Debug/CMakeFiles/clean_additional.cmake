# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\ResearchManagementSystem_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\ResearchManagementSystem_autogen.dir\\ParseCache.txt"
  "ResearchManagementSystem_autogen"
  )
endif()
