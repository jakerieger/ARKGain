cmake_minimum_required(VERSION 3.14.0)

option(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES "Enable VST 3 Plug-in Examples" OFF)
option(SMTG_ENABLE_VST3_HOSTING_EXAMPLES "Enable VST 3 Hosting Examples" OFF)

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

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

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(ARKGain
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
        source/Win32Window.cpp
        source/Win32Window.h
)

smtg_target_add_plugin_snapshots(ARKGain
        RESOURCES
        resource/92F2B27B99945A8BBD45AC45A00B357A_snapshot.png
        resource/92F2B27B99945A8BBD45AC45A00B357A_snapshot_2.0x.png
)

smtg_target_add_plugin_resource(ARKGain resource/ui.svg)

find_package(NanoSVG CONFIG REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)


target_link_libraries(ARKGain
        PRIVATE
        sdk
        NanoSVG::nanosvg
        OpenGL::GL
        OpenGL::GLU
)

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
