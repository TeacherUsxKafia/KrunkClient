cmake_minimum_required(VERSION 3.16)
project(OpenKrunker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# THIS FIXES THE ERROR: Tells the compiler to use Unicode
add_definitions(-DUNICODE -D_UNICODE)

# Automatically get WebView2 and WIL
include(FetchContent)
FetchContent_Declare(
    Microsoft_Web_WebView2
    URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42
)
FetchContent_Declare(
    Microsoft_Windows_ImplementationLibrary
    URL https://www.nuget.org/api/v2/package/Microsoft.Windows.ImplementationLibrary/1.0.220201.1
)
FetchContent_MakeAvailable(Microsoft_Web_WebView2 Microsoft_Windows_ImplementationLibrary)

add_executable(KrunkerClient WIN32 main.cpp)

# Link Windows libraries
target_link_libraries(KrunkerClient PRIVATE WindowsApp)
target_include_directories(KrunkerClient PRIVATE ${microsoft_web_webview2_SOURCE_DIR}/build/native/include)
target_include_directories(KrunkerClient PRIVATE ${microsoft_windows_implementationlibrary_SOURCE_DIR}/include)
