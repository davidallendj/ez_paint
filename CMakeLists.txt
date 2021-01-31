cmake_minimum_required(VERSION 3.12)
project(sp_hash VERSION 0.1.0)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

include(CTest)
enable_testing()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(Threads REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(PNG REQUIRED)
# find_package(tilefiledialogs CONFIG REQUIRED)

file(GLOB SRC CONFIGURE_DEPENDS "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
target_sources(${PROJECT_NAME} PRIVATE ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE 
	sfml-system 
	sfml-window 
	sfml-graphics 
	Threads::Threads 
	ImGui-SFML::ImGui-SFML
	PNG::PNG 
	# tinyfiledialogs::tinyfiledialogs
)
target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE)
target_include_directories(${PROJECT_NAME} PUBLIC "include")

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILTER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
