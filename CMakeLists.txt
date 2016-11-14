project(actasp)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE RelWithDebInfo)
#set(CMAKE_BUILD_TYPE Debug)


#change this to run the test 
set(UNITTESTPP_PATH "unittest++/UnitTest++.h")

include(CheckIncludeFileCXX)

include_directories(include)

 SET(actasp_SRC)

add_subdirectory(src)

add_library(actasp SHARED ${actasp_SRC})

# compiles tests if UnitTest++ is available
UNSET(HAVE_UNITTESTXX CACHE)
CHECK_INCLUDE_FILE_CXX(${UNITTESTPP_PATH} HAVE_UNITTESTXX)

IF(HAVE_UNITTESTXX)

        execute_process(COMMAND clingo -v OUTPUT_VARIABLE CLINGO_VERSION)
           
         #we only support clingo version 3 or 4
         string(REGEX MATCH "[34]" VERSION_FOUND "${CLINGO_VERSION}")
         
         message(STATUS "Clingo version ${VERSION_FOUND} found")
         
         string(COMPARE EQUAL ${VERSION_FOUND} "3" CLINGO3_FOUND)
         
         IF(CLINGO3_FOUND)
	  set(TEST_SOURCE_DIR "test/src/clingo3")
	 ELSE(CLINGO3_FOUND)
	  set(TEST_SOURCE_DIR "test/src/clingo4")
	 ENDIF(CLINGO3_FOUND)

	add_executable(testAll  ${TEST_SOURCE_DIR}/main.cpp
                          ${TEST_SOURCE_DIR}/TestReasoner.cpp
                          ${TEST_SOURCE_DIR}/TestAnswerSet.cpp 
                          ${TEST_SOURCE_DIR}/TestAspFluent.cpp 
                          ${TEST_SOURCE_DIR}/TestIsLocalyOptimal.cpp)
	target_link_libraries(testAll actasp libUnitTest++.a)

ENDIF()
