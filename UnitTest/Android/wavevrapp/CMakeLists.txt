# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(SD_ENGINE_DIR ../../..)
set(SD_THIRD_PARTY_DIR ${SD_ENGINE_DIR}/ThirdParty)
set(SD_ENGINE_INC_DIR ${SD_ENGINE_DIR}/SourceCode/include)
set(SD_TP_ASSIMP_INC_DIR ${SD_THIRD_PARTY_DIR}/Assimp/include)
set(SD_TP_GLM_INC_DIR ${SD_THIRD_PARTY_DIR}/GLM)
set(SD_TP_IMGUI_INC_DIR ${SD_THIRD_PARTY_DIR}/ImGui/include)

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
        ${SD_ENGINE_INC_DIR}/Basic/Exception
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
		${SD_ENGINE_INC_DIR}/Basic/ECS
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
        ${SD_ENGINE_INC_DIR}/Graphics/Scene
        ${SD_ENGINE_INC_DIR}/Graphics/Vec
        ${SD_ENGINE_INC_DIR}/Graphics/VertexBuffer
        ${SD_ENGINE_INC_DIR}/Graphics/IndexBuffer
        ${SD_ENGINE_INC_DIR}/Graphics/Texture
        ${SD_ENGINE_INC_DIR}/Graphics/ModelImporter
        ${SD_ENGINE_INC_DIR}/Graphics/Mesh
        ${SD_ENGINE_INC_DIR}/Graphics/Material
        ${SD_ENGINE_INC_DIR}/Graphics/ShaderProgram
        ${SD_ENGINE_INC_DIR}/Graphics/RenderFlow
        ${SD_ENGINE_INC_DIR}/Graphics/CommandBuffer
		${SD_ENGINE_INC_DIR}/Graphics/BasicComponent
        ${SD_ENGINE_INC_DIR}/Graphics/EmbeddedShaderProgramSource
        ${SD_ENGINE_INC_DIR}/GUI
        ${SD_ENGINE_INC_DIR}/Physics/Ray)

set (WAVE_SDK_DIR ${VR_SDK_ROOT}/include)

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

# thrid party lib ----- wavesdk.
add_library( # Sets the name of the library.
        wvr_api
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        IMPORTED )

set_target_properties( wvr_api PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/build/${WVR_AAR}/jni/${ANDROID_ABI}/libwvr_api.so )

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
        ../../Common/UnitTest_FeatureDeveloping/Code)

include_directories(
        ${SD_TP_ASSIMP_INC_DIR}
        ${SD_TP_GLM_INC_DIR}
        ${SD_TP_IMGUI_INC_DIR}
        ${SD_ENGINE_INC_DIRS}
        ${WAVE_SDK_DIR}
        ${COMMON_INC_DIR})

add_library( # Sets the name of the library.
             sdengine_native

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/android_jni_main.cpp
             src/main/cpp/WaveVRApplication.cpp
             src/main/cpp/WaveVRVulkanManager.cpp
             src/main/cpp/WaveSystem.cpp
             src/main/cpp/WaveVREventArg.cpp
             src/main/cpp/WaveVRControllerComponent.cpp
             ../../Common/UnitTest_FeatureDeveloping/Code/HUDComponent.cpp
             ../../Common/UnitTest_FeatureDeveloping/Code/SampleDrawObjects.cpp
             ../../Common/UnitTest_FeatureDeveloping/Code/ScreenRayComponent.cpp
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
                       wvr_api
                       -llog
                       -landroid
                       -lEGL
                       -lGLESv3)