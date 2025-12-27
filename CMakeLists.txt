cmake_minimum_required(VERSION 3.19)
project(modsync LANGUAGES CXX)

if(NOT DEFINED ENV{SERVER_URL})
    message(FATAL_ERROR "ENV SERVER_URL not set")
endif()
set(SERVERURL "$ENV{SERVER_URL}")

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(modsync
    WIN32 MACOSX_BUNDLE
        src/main.cpp
        src/CreateInstanceAction.cpp
        src/CreateInstanceAction.h
        src/Downloader.cpp
        src/Downloader.h
        src/Initialise.cpp
        src/Initialise.h
        src/SyncMetadata.h
        src/Locations.h
        src/LoaderInstaller.cpp
        src/LoaderInstaller.h
        src/LoaderInstaller.cpp
        src/FileSyncer.cpp
        src/FileSyncer.h
        src/SyncAction.h
        src/UpdateModsAction.cpp
        src/UpdateModsAction.h
)

target_link_libraries(modsync
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

target_compile_definitions(modsync PRIVATE SERVERURL="${SERVERURL}")

include(GNUInstallDirs)

install(TARGETS modsync
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET modsync
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
