cmake_minimum_required(VERSION 3.16)
project(adventofcode LANGUAGES CXX VERSION 1.0)

set(CUDA_ARCH 70)

##########################################################################
# Include Directories
include_directories(include)

##########################################################################
# Compile Flags
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_definitions(
        $<$<CONFIG:RELEASE>:NDEBUG>
        $<$<CONFIG:RELEASE>:BOOST_DISABLE_ASSERTS>
)

##########################################################################
# Day 01
add_executable(main_01
	src/main01.cpp)
if (MSVC)
   target_compile_options(main_01 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_01 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 02
add_executable(main_02
 src/main02.cpp)
if (MSVC)
   target_compile_options(main_02 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_02 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 03
add_executable(main_03
 src/main03.cpp)
if (MSVC)
   target_compile_options(main_03 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_03 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 04
add_executable(main_04
 src/main04.cpp)
if (MSVC)
   target_compile_options(main_04 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_04 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 05
add_executable(main_05
 src/main05.cpp)
if (MSVC)
   target_compile_options(main_05 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_05 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 06
add_executable(main_06
 src/main06.cpp)
if (MSVC)
   target_compile_options(main_06 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_06 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

set_target_properties(main_01 main_02 main_03 main_04 main_05 main_06
                      PROPERTIES CXX_STANDARD 20)
