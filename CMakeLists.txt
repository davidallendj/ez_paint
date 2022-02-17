cmake_minimum_required(VERSION 3.12)
project(ez_paint VERSION 0.1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

include(CTest)
enable_testing()


# add_subdirectory("submodules/imgui-sfml")
# add_subdirectory("submodules/cxxopts")
set(CMAKE_EXPORT_COMPILE_COMMAND 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
find_package(Threads REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(DataFrame CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(Backward)
find_package(Catch2 CONFIG REQUIRED)
find_package(libheif CONFIG REQUIRED)
# find_package(tilefiledialogs CONFIG REQUIRED)

file(GLOB SRC CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB TESTS CONFIGURE_DEPENDS "tests/*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
target_sources(${PROJECT_NAME} PRIVATE ${SRC})
target_link_libraries(${PROJECT_NAME} 
PRIVATE 
	sfml-system 
	sfml-window 
	sfml-graphics 
	Threads::Threads
	imgui::imgui
	implot::implot
	ImGui-SFML::ImGui-SFML
	PNG::PNG
	fmt::fmt
	DataFrame::DataFrame
	Catch2::Catch2
	heif
PUBLIC 
	Backward::Backward
	-ldw 
	-lbfd 
	-ldwarf 
	-lnuma
	# tinyfiledialogs::tinyfiledialogs
)

include(Catch)
target_include_directories(${PROJECT_NAME} 
PUBLIC 
	"include" 
	"submodules/imgui-filebrowser" 
	"submodules/cxxopts/include"
PRIVATE
	${RAPIDJSON_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC RELEASE)
target_compile_options(${PROJECT_NAME} PUBLIC 
	-g
	-std=c++20
	-Ofast
	-Wall
	-Wextra
	-Wno-switch
	-Wno-unused-variable
	-Wno-unused-parameter
	-Wno-sign-conversion
	-Wno-unused-private-field
	-pedantic-errors
	-fmodules-ts
	-fPIC
	-fPIE
)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_BUILD_TYPE "Debug")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# set_target_properties(${PROJECT_NAME} PROPERTIES
# 	CMAKE_BUILD_TYPE Debug
# 	CMAKE_CXX_COMPILER /usr/bin/clang++
# 	CMAKE_CXX_STANDARD_REQUIRED ON
# 	CPACK_PROJECT_NAME ${PROJECT_NAME}
# 	CPACK_PROJECT_VERSION ${PROJECT_VERSION}
# )

add_custom_target("${PROJECT_NAME}-tests" SOURCE ${TESTS})
include(CPack)
