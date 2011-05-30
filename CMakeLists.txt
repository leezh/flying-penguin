project (flying-penguin CXX)
cmake_minimum_required (VERSION 2.6)

# == Variables ==

if (WIN32)
    set (RESOURCE_DIR "share/flying-penguin/" CACHE FILEPATH "Directory to look for resources")
else ()
    set (RESOURCE_DIR "${CMAKE_INSTALL_PREFIX}/share/flying-penguin/" CACHE FILEPATH "Directory to look for resources")
endif ()
set (RESOURCE_DIR_REL "../share/flying-penguin/" CACHE FILEPATH "Relative paths for use in tarballs")

# == Source Files ==

# Use this to generate:
# find src -type f -name "*.[h|c]*" | grep -v config.h.in | sort
set (project_sources
    src/apps.cpp
    src/apps.hpp
    src/background.cpp
    src/background.hpp
    src/ConfigFile/ConfigFile.cpp
    src/ConfigFile/ConfigFile.h
    src/entities.cpp
    src/entities.hpp
    src/main.cpp
    src/main.hpp
    src/npc.cpp
    src/npc.hpp
    src/particles.cpp
    src/particles.hpp
    src/penguin.cpp
    src/penguin.hpp
    src/record.cpp
    src/record.hpp
    src/resources.cpp
    src/resources.hpp
    src/util.cpp
    src/util.hpp
    src/world.cpp
    src/world.hpp)
configure_file("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_BINARY_DIR}/config.h")
if (WIN32)
    if (MINGW)
        set (project_sources ${project_sources} misc/windows/flying-penguin-mingw.rc)
    else ()
        set (project_sources ${project_sources} misc/windows/flying-penguin.rc)
    endif ()
endif ()
add_executable (flying-penguin ${project_sources})

# == Libraries ==

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/misc/cmake")
if (WIN32)
    set (CMAKE_BUILD_TYPE Release)
    add_definitions(-DSFML_DYNAMIC)
endif ()
find_package (SFML REQUIRED COMPONENTS Audio Graphics Window System)
find_package (PhysFS REQUIRED)
include_directories (${SFML_INCLUDE_DIR} ${PHYSFS_INCLUDE_DIR} ${PROJECT_BINARY_DIR})
target_link_libraries (flying-penguin ${SFML_LIBRARY} ${PHYSFS_LIBRARY})

# == Package Info ==

install (TARGETS flying-penguin DESTINATION bin)
install (DIRECTORY share/flying-penguin/ DESTINATION share/flying-penguin)
install (FILES COPYING README.markdown DESTINATION share/flying-penguin)
if (UNIX)
    install (FILES misc/linux/flying-penguin.png DESTINATION share/pixmaps)
    install (FILES misc/linux/flying-penguin.desktop DESTINATION share/applications)
endif ()
if (WIN32 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/misc/windows/bin" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/misc/windows/bin")
    install (DIRECTORY misc/windows/bin/ DESTINATION bin)
endif ()

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_DESCRIPTION "An arcade game involving a penguin, strap-on wings and flying fish")
set (CPACK_PACKAGE_VENDOR "Lee Zher Huei")
set (CPACK_PACKAGE_CONTACT "lee.zh.92@gmail.com")
set (CPACK_PACKAGE_VERSION "0.2")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "2")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")

set (CPACK_NSIS_DISPLAY_NAME "The Flying Penguin")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "The Flying Penguin")
set (CPACK_PACKAGE_EXECUTABLES "flying-penguin;The Flying Penguin")
set (CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\flying-penguin.exe")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\orange-install-nsis.ico")
set (CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\orange-uninstall-nsis.ico")
set (CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\misc\\\\windows\\\\nsis\\\\banner.bmp")

set (CPACK_DEBIAN_PACKAGE_MAINTAINER "Lee Zher Huei <lee.zh.92@gmail.com>")
set (CPACK_DEBIAN_PACKAGE_SECTION "games")
set (CPACK_DEBIAN_PRIORITY "optional")
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-system1.6, libsfml-window1.6, libsfml-graphics1.6, libsfml-audio1.6, libgl1, libglu1, libstdc++6")

include (CPack)

