# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RadarSim_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RadarSim_autogen.dir\\ParseCache.txt"
  "RadarSim_autogen"
  )
endif()
