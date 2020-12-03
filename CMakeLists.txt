cmake_minimum_required(VERSION 3.16)
project(adventofcode LANGUAGES CXX CUDA VERSION 1.0)

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
	src/main01.cu)
set_property(TARGET main_01 PROPERTY CUDA_ARCHITECTURES ${CUDA_ARCH})
target_compile_options(main_01 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>: --resource-usage --ptxas-options=-v -lineinfo $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>>)
if (CUDA_BUILD_DEBUG)
	target_compile_options(main_01 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif ()

##########################################################################
# Day 02
add_executable(main_02
 src/main02.cpp)
set_property(TARGET main_02 PROPERTY CUDA_ARCHITECTURES OFF)
if (MSVC)
   target_compile_options(main_02 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_02 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 03
add_executable(main_03
 src/main03.cpp)
set_property(TARGET main_03 PROPERTY CUDA_ARCHITECTURES OFF)
if (MSVC)
   target_compile_options(main_03 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_03 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

set_target_properties(main_01 main_02 main_03
                      PROPERTIES CXX_STANDARD 20)
