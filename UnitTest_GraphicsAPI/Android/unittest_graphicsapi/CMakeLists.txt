# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(SD_ENGINE_DIR ../../..)
set(SD_THIRD_PARTY_DIR ${SD_ENGINE_DIR}/ThirdParty)
set(SD_ENGINE_INC_DIR ${SD_ENGINE_DIR}/SourceCode/include)
set(SD_TP_ASSIMP_INC_DIR ${SD_THIRD_PARTY_DIR}/Assimp/include)
set(SD_TP_GLM_INC_DIR ${SD_THIRD_PARTY_DIR}/GLM)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(Configuration "Debug")
    message (STATUS "Debug Build")
else()
    set(Configuration,"Release")
    message (STATUS "Release Build")
endif()

set (SD_ENGINE_INC_DIRS
        ${SD_ENGINE_INC_DIR}
        ${SD_ENGINE_INC_DIR}/
        ${SD_ENGINE_INC_DIR}/Math
        ${SD_ENGINE_INC_DIR}/Basic
        ${SD_ENGINE_INC_DIR}/Basic/LogManager
        ${SD_ENGINE_INC_DIR}/Basic/LogManager/Android
        ${SD_ENGINE_INC_DIR}/Basic/Timer
        ${SD_ENGINE_INC_DIR}/Basic/Timer/Android
        ${SD_ENGINE_INC_DIR}/Basic/FileSystem
        ${SD_ENGINE_INC_DIR}/Basic/FileSystem/Android
        ${SD_ENGINE_INC_DIR}/Basic/EventSystem
        ${SD_ENGINE_INC_DIR}/Basic/Application
        ${SD_ENGINE_INC_DIR}/Basic/Object
        ${SD_ENGINE_INC_DIR}/Basic/KeyMap
        ${SD_ENGINE_INC_DIR}/Graphics
        ${SD_ENGINE_INC_DIR}/Graphics/BasicShapeCreator
        ${SD_ENGINE_INC_DIR}/Graphics/Resolution
        ${SD_ENGINE_INC_DIR}/Graphics/Manager
        ${SD_ENGINE_INC_DIR}/Graphics/ManagerIdentity
        ${SD_ENGINE_INC_DIR}/Graphics/Manager/GLES3
        ${SD_ENGINE_INC_DIR}/Graphics/Manager/Vulkan
        ${SD_ENGINE_INC_DIR}/Graphics/Manager/Vulkan/Core
        ${SD_ENGINE_INC_DIR}/Graphics/ManagerParam
        ${SD_ENGINE_INC_DIR}/Graphics/ManagerParam/GLES3
        ${SD_ENGINE_INC_DIR}/Graphics/ManagerParam/Vulkan
        ${SD_ENGINE_INC_DIR}/Graphics/ImageLoader
        ${SD_ENGINE_INC_DIR}/Graphics/Vec
        ${SD_ENGINE_INC_DIR}/Graphics/VertexBuffer
        ${SD_ENGINE_INC_DIR}/Graphics/IndexBuffer
        ${SD_ENGINE_INC_DIR}/Graphics/Texture
        ${SD_ENGINE_INC_DIR}/Graphics/ModelImporter
        ${SD_ENGINE_INC_DIR}/Graphics/Mesh
        ${SD_ENGINE_INC_DIR}/Graphics/Material
        ${SD_ENGINE_INC_DIR}/Graphics/RenderFlow
        ${SD_ENGINE_INC_DIR}/Graphics/CommandBuffer/)

message(STATUS "SDEngineRootDir: ${SD_ENGINE_DIR}")
message(STATUS "SDThirdPartyRootDir: ${SD_THIRD_PARTY_DIR}")

message(STATUS "SDEngineIncludeDir: ${SD_ENGINE_INC_DIR}")
message(STATUS "SDAssimpIncludeDir: ${SD_TP_ASSIMP_INC_DIR}")
message(STATUS "SDGLMDir: ${SD_TP_GLM_INC_DIR}")
message(STATUS "SDEngineIncDirs: ${SD_ENGINE_INC_DIRS}")

message(STATUS "CurrentSourceDir: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "RuntimeOutputDir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "LibraryOutputDir: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "LibraryPaths: ${CMAKE_LIBRARY_PATH}")
message(STATUS "BuildType: ${CMAKE_BUILD_TYPE}")
message(STATUS "AndroidABI: ${ANDROID_ABI}")
message(STATUS "ImportLibPath: ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}")

# third party library ----- assimp
add_library( # Sets the name of the library.
        assimp
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        IMPORTED )

set_target_properties( assimp PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libassimp.so )

# Engine library
add_library( # Sets the name of the library.
        SDEngine
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        IMPORTED )

set_target_properties( SDEngine PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libSDEngine.so )

add_library( # Sets the name of the library.
        SDEngine_AndroidApplication
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        IMPORTED )

set_target_properties( SDEngine_AndroidApplication PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libSDEngine_AndroidApplication.so )

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(COMMON_INC_DIR
        ../../Common/UnitTest_VulkanAPI/Code)

include_directories(
        ${SD_TP_ASSIMP_INC_DIR}
        ${SD_TP_GLM_INC_DIR}
        ${SD_ENGINE_INC_DIRS}
        ${COMMON_INC_DIR})

add_library( # Sets the name of the library.
             sdengine_native

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/android_jni_main.cpp
             src/main/cpp/TestAndroidApplication.cpp
             ../../Common/UnitTest_VulkanAPI/Code/Sample.cpp
             ../../Common/UnitTest_VulkanAPI/Code/Sample1_DrawTriangle.cpp
             ../../Common/UnitTest_VulkanAPI/Code/Sample2_DrawScene.cpp
             ../../Common/UnitTest_VulkanAPI/Code/Sample3_MultiSubpass.cpp
             ../../Common/UnitTest_VulkanAPI/Code/Sample4_DrawObjects.cpp
             ../../Common/UnitTest_VulkanAPI/Code/VulkanAPITestManager.cpp
             ../../Common/UnitTest_VulkanAPI/Code/VulkanAPITestManager_ScreenRenderer.cpp
             ../../Common/UnitTest_VulkanAPI/Code/CommandRecordingThread.cpp
             ../../Common/UnitTest_VulkanAPI/Code/CommandRecordingThreadV2.cpp
        )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       sdengine_native

                       # Links the target library to the log library
                       # included in the NDK.
                       assimp
                       SDEngine
                       SDEngine_AndroidApplication
                       -llog
                       -landroid
                       -lEGL
                       -lGLESv3)