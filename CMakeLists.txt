cmake_minimum_required(VERSION 3.22)

project(VoxMaster
    VERSION 0.1.0
    DESCRIPTION "AI-assisted vocal mixing plugin"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

option(VOXMASTER_FETCH_JUCE "Fetch JUCE with FetchContent when VOXMASTER_JUCE_SOURCE_DIR is not set" ON)
set(VOXMASTER_JUCE_SOURCE_DIR "" CACHE PATH "Path to a local JUCE checkout")
set(VOXMASTER_JUCE_GIT_TAG "8.0.8" CACHE STRING "JUCE git tag/branch to fetch when FetchContent is enabled")
set(VOXMASTER_ONNXRUNTIME_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/onnxruntime" CACHE PATH "Path to the ONNX Runtime package root")

if(VOXMASTER_JUCE_SOURCE_DIR)
    add_subdirectory("${VOXMASTER_JUCE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/juce")
elseif(VOXMASTER_FETCH_JUCE)
    FetchContent_Declare(
        JUCE
        GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
        GIT_TAG ${VOXMASTER_JUCE_GIT_TAG}
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(JUCE)
else()
    find_package(JUCE CONFIG REQUIRED)
endif()

if(APPLE)
    set(VOXMASTER_PLUGIN_FORMATS VST3 AU Standalone)
else()
    set(VOXMASTER_PLUGIN_FORMATS VST3 Standalone)
endif()

juce_add_plugin(VoxMaster
    COMPANY_NAME "VoxMasterAudio"
    BUNDLE_ID "com.voxmasteraudio.voxmaster"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE VxMa
    PLUGIN_CODE VxMx
    FORMATS ${VOXMASTER_PLUGIN_FORMATS}
    PRODUCT_NAME "VoxMaster"
)

juce_generate_juce_header(VoxMaster)

target_sources(VoxMaster
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/core/LoggerThread.cpp
        Source/core/SharedState.cpp
        Source/dsp/DSPManager.cpp
        Source/dsp/InputCalibrationProcessor.cpp
        Source/dsp/EQProcessor.cpp
        Source/dsp/DynamicEQProcessor.cpp
        Source/dsp/DeEsserProcessor.cpp
        Source/dsp/CompressorProcessor.cpp
        Source/dsp/ToneMatchProcessor.cpp
        Source/dsp/SafetyLimiter.cpp
        Source/dsp/FeatureExtractor.cpp
        Source/ai/InferenceEngine.cpp
        Source/ai/InferenceThread.cpp
        Source/ai/TonalBalanceMatcher.cpp
        Source/reference/ReferenceAnalyzer.cpp
        Source/reference/ReferenceAnalysisThread.cpp
        Source/feedback/FeedbackRecorder.cpp
        Source/feedback/FeedbackWriterThread.cpp
        Source/ui/MainComponent.cpp
)

target_include_directories(VoxMaster
    PRIVATE
        Source
        Source/core
        Source/dsp
        Source/ai
        Source/reference
        Source/feedback
        Source/ui
)

target_compile_definitions(VoxMaster
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        VOXMASTER_ENABLE_ONNXRUNTIME=1
)

target_link_libraries(VoxMaster
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_gui_extra
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

set(VOXMASTER_ONNXRUNTIME_INCLUDE_DIR "${VOXMASTER_ONNXRUNTIME_DIR}/include")

if(WIN32)
    set(VOXMASTER_ONNXRUNTIME_LIBRARY "${VOXMASTER_ONNXRUNTIME_DIR}/lib/onnxruntime.lib")
    set(VOXMASTER_ONNXRUNTIME_RUNTIME "${VOXMASTER_ONNXRUNTIME_DIR}/bin/onnxruntime.dll")
elseif(APPLE)
    set(VOXMASTER_ONNXRUNTIME_LIBRARY "${VOXMASTER_ONNXRUNTIME_DIR}/lib/libonnxruntime.dylib")
    set(VOXMASTER_ONNXRUNTIME_RUNTIME "${VOXMASTER_ONNXRUNTIME_LIBRARY}")
else()
    set(VOXMASTER_ONNXRUNTIME_LIBRARY "${VOXMASTER_ONNXRUNTIME_DIR}/lib/libonnxruntime.so")
    set(VOXMASTER_ONNXRUNTIME_RUNTIME "${VOXMASTER_ONNXRUNTIME_LIBRARY}")
endif()

if(EXISTS "${VOXMASTER_ONNXRUNTIME_INCLUDE_DIR}" AND EXISTS "${VOXMASTER_ONNXRUNTIME_LIBRARY}")
    target_include_directories(VoxMaster PRIVATE "${VOXMASTER_ONNXRUNTIME_INCLUDE_DIR}")
    target_link_libraries(VoxMaster PRIVATE "${VOXMASTER_ONNXRUNTIME_LIBRARY}")
    target_compile_definitions(VoxMaster PRIVATE VOXMASTER_ONNXRUNTIME_AVAILABLE=1)

    if(EXISTS "${VOXMASTER_ONNXRUNTIME_RUNTIME}")
        add_custom_command(TARGET VoxMaster POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${VOXMASTER_ONNXRUNTIME_RUNTIME}"
                "$<TARGET_FILE_DIR:VoxMaster>"
            COMMENT "Copying ONNX Runtime runtime library next to the VoxMaster binary"
        )
    endif()
else()
    message(WARNING "ONNX Runtime was not found at ${VOXMASTER_ONNXRUNTIME_DIR}; VoxMaster will configure without linking ONNX Runtime. Place headers in external/onnxruntime/include and libraries in external/onnxruntime/lib, or set VOXMASTER_ONNXRUNTIME_DIR.")
    target_compile_definitions(VoxMaster PRIVATE VOXMASTER_ONNXRUNTIME_AVAILABLE=0)
endif()

if(MSVC)
    target_compile_options(VoxMaster PRIVATE /W4 /permissive- /EHsc $<$<CONFIG:Release>:/O2> $<$<CONFIG:RelWithDebInfo>:/O2>)
else()
    target_compile_options(VoxMaster PRIVATE -Wall -Wextra -Wpedantic $<$<CONFIG:Release>:-O3> $<$<CONFIG:RelWithDebInfo>:-O3>)
endif()

set_target_properties(VoxMaster PROPERTIES
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++20"
    XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++"
)
