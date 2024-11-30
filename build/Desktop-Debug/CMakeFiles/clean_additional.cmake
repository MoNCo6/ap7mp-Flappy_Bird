# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Flappy_Bird_Game_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Flappy_Bird_Game_autogen.dir/ParseCache.txt"
  "Flappy_Bird_Game_autogen"
  )
endif()
