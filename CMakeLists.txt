cmake_minimum_required(VERSION 3.19)
project(modsync LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(modsync
    WIN32 MACOSX_BUNDLE
        src/main.cpp
        src/ui/WelcomePage.cpp
        src/ui/WelcomePage.h
        src/ui/MainWizard.cpp
        src/ui/MainWizard.h
        src/SyncClient.cpp
        src/SyncClient.h
        src/ui/SyncPage.cpp
        src/ui/SyncPage.h
        src/Downloader.cpp
        src/Downloader.h
)

target_link_libraries(modsync
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Network
)

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
