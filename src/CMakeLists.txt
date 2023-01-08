cmake_minimum_required (VERSION 3.8)

set(Src
	"ConverterJSON.cpp"
	"SearchServer.cpp"
	"InvertedIndex.cpp"
	"main.cpp"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_executable(searchEngine ${Src})

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

target_link_libraries(searchEngine nlohmann_json::nlohmann_json)
target_include_directories(searchEngine PUBLIC ${MY_INCLUDE_DIR})