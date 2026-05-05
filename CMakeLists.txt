cmake_minimum_required(VERSION 3.16)
project(OpenKrunker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DUNICODE -D_UNICODE)

include(FetchContent)
FetchContent_Declare(
    wv2
    URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42
)
FetchContent_Declare(
    wil
    URL https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.220201.1
)
FetchContent_MakeAvailable(wv2 wil)

add_executable(KrunkerClient WIN32 main.cpp)

# Explicitly find the library file to ensure it exists
file(GLOB_RECURSE WV2_LIB "${wv2_SOURCE_DIR}/**/x64/WebView2Loader.lib")

target_include_directories(KrunkerClient PRIVATE 
    ${wv2_SOURCE_DIR}/build/native/include
    ${wil_SOURCE_DIR}/include
)

# This is the strongest way to link a specific file in CMake
target_link_libraries(KrunkerClient PRIVATE WindowsApp "${WV2_LIB}")
