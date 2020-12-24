cmake_minimum_required(VERSION 3.16)
project(adventofcode LANGUAGES CXX CUDA VERSION 1.0)

##########################################################################
# Include Directories
include_directories(include)
include_directories(${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES})

find_package(CUDAToolkit REQUIRED)

##########################################################################
# Compile Flags


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

##########################################################################
# Day 07
add_executable(main_07
 src/main07.cpp)
if (MSVC)
   target_compile_options(main_07 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_07 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 08
add_executable(main_08
 src/main08.cpp)
if (MSVC)
   target_compile_options(main_08 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_08 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 09
add_executable(main_09
 src/main09.cpp)
if (MSVC)
   target_compile_options(main_09 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_09 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 10
add_executable(main_10
 src/main10.cpp)
if (MSVC)
   target_compile_options(main_10 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_10 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 11
add_executable(main_11
 src/main11.cpp)
if (MSVC)
   target_compile_options(main_11 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_11 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 12
add_executable(main_12
 src/main12.cpp)
if (MSVC)
   target_compile_options(main_12 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_12 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 13
add_executable(main_13
 src/main13.cu)
set_property(TARGET main_13 PROPERTY CUDA_ARCHITECTURES 70)
target_compile_options(main_13 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>: --resource-usage --ptxas-options=-v -lineinfo $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>>)
if (CUDA_BUILD_DEBUG)
	target_compile_options(main_13 PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-G>)
endif ()

##########################################################################
# Day 14
add_executable(main_14
 src/main14.cpp)
if (MSVC)
   target_compile_options(main_14 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_14 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 15
add_executable(main_15
 src/main15.cpp)
if (MSVC)
   target_compile_options(main_15 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_15 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 16
add_executable(main_16
 src/main16.cpp)
if (MSVC)
   target_compile_options(main_16 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_16 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 17
add_executable(main_17
 src/main17.cpp)
if (MSVC)
   target_compile_options(main_17 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_17 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 18
add_executable(main_18
 src/main18.cpp)
if (MSVC)
   target_compile_options(main_18 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_18 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 19
add_executable(main_19
 src/main19.cpp)
if (MSVC)
   target_compile_options(main_19 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_19 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 20
add_executable(main_20
 src/main20.cpp)
if (MSVC)
   target_compile_options(main_20 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_20 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 21
add_executable(main_21
 src/main21.cpp)
if (MSVC)
   target_compile_options(main_21 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_21 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 22
add_executable(main_22
 src/main22.cpp)
if (MSVC)
   target_compile_options(main_22 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_22 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 23
add_executable(main_23
 src/main23.cpp)
if (MSVC)
   target_compile_options(main_23 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_23 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

##########################################################################
# Day 24
add_executable(main_24
 src/main24.cpp)
if (MSVC)
   target_compile_options(main_24 PRIVATE /W4 $<$<CONFIG:RELEASE>:/O2> $<$<CONFIG:DEBUG>:/Od>)
else()
   target_compile_options(main_24 PRIVATE -Wall -Wextra -Werror $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:DEBUG>:-O0>)
endif()

set_target_properties(main_01 main_02 main_03 main_04 main_05 main_06 main_07 main_08 main_09 main_10 main_11 main_12 main_13 main_14 main_15 main_16 main_17 main_18 main_19 main_20 main_21 main_22 main_23 main_24
                      PROPERTIES CXX_STANDARD 20)
