cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)
set(VSTGUI_DISABLE ON CACHE BOOL "Disable VSTGUI")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

# Change this to the path of your vcpkg install
set(VCPKG_CMAKE_PATH "C:/Users/conta/.vcpkg-clion/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_CMAKE_PATH} CACHE STRING "Vcpkg toolchain file")

# Ensure vcpkg installs static libs; Dynamic libs cannot be linked to VST3 projects
set(VCPKG_TARGET_TRIPLET x64-windows-static)

set(vst3sdk_SOURCE_DIR "C:/VST_SDK/vst3sdk")
if (NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif ()

project(ARKGain
        # This is your plug-in version number. Change it here only.
        # Version number symbols usable in C++ can be found in
        # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
        VERSION 1.0.0.0
        DESCRIPTION "ARKGain VST 3 Plug-in"
)

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
add_subdirectory(ArkVector)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(ARKGain
        # plugin sources
        source/version.h
        source/CIDs.h
        source/Processor.h
        source/Processor.cpp
        source/Controller.h
        source/Controller.cpp
        source/Factory.cpp
        source/View.h
        source/View.cpp
        source/Platform.h
        source/ParameterIds.h
        source/Canvas.h
        source/Canvas.cpp
)

smtg_target_add_plugin_snapshots(ARKGain
        RESOURCES
        resource/92F2B27B99945A8BBD45AC45A00B357A_snapshot.png
        resource/92F2B27B99945A8BBD45AC45A00B357A_snapshot_2.0x.png
)

smtg_target_add_plugin_resource(ARKGain resource/ui.svg)

include_directories(ArkVector/Lib)

target_link_libraries(ARKGain
        PRIVATE
        sdk
        ArkVector
)

#include_directories(
#        ${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/x64-windows-static/include
#)

smtg_target_configure_version_file(ARKGain)

if (SMTG_MAC)
    smtg_target_set_bundle(ARKGain
            BUNDLE_IDENTIFIER com.arksyn.gain
            COMPANY_NAME "ARKSYN Audio"
    )
    smtg_target_set_debug_executable(ARKGain
            "/Applications/VST3PluginTestHost.app"
            "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif (SMTG_WIN)
    target_sources(ARKGain PRIVATE
            resource/win32resource.rc
    )
    if (MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ARKGain)

        smtg_target_set_debug_executable(ARKGain
                "C:/Program Files/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
                "--pluginfolder \"$(OutDir)/\""
        )
    endif ()
endif (SMTG_MAC)
