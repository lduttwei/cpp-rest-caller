cmake_minimum_required(VERSION 3.11)

project(cpp_caller VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_executable(cpp_caller main.cpp)

include(FetchContent)

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG f4622efcb59d84071ae11404ae61bd821c1c344b) # the commit hash for 1.6.2
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.7.3) # the commit hash for 1.6.2
FetchContent_MakeAvailable(json)


configure_file(main.hpp.in main.hpp)

target_include_directories(cpp_caller PUBLIC
	"${PROJECT_BINARY_DIR}"
	)


	
target_link_libraries(cpp_caller 
	PRIVATE nlohmann_json::nlohmann_json
	PRIVATE cpr::cpr
	)
