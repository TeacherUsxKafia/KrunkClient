cmake_minimum_required(VERSION 3.16)
project(OpenKrunker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DUNICODE -D_UNICODE)

include(FetchContent)
FetchContent_Declare(
    webview2
    URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42
)
FetchContent_Declare(
    wil
    URL https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.220201.1
)
FetchContent_MakeAvailable(webview2 wil)

add_executable(KrunkerClient WIN32 main.cpp)

# This helps the compiler find the .h files
target_include_directories(KrunkerClient PRIVATE 
    ${webview2_SOURCE_DIR}/build/native/include
    ${wil_SOURCE_DIR}/include
)

# This tells the compiler EXACTLY where to find the missing library file
link_directories(${webview2_SOURCE_DIR}/build/native/x64)
target_link_libraries(KrunkerClient PRIVATE WindowsApp WebView2Loader.lib)
