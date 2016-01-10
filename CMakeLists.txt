cmake_minimum_required(VERSION 2.8)
project(fwknop-gui CXX C)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
if(NOT(DEFINED IS_WIN32 OR DEFINED IS_LINUX OR DEFINED IS_APPLE))
  if(WIN32)
    set(IS_WIN32 TRUE)
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(IS_APPLE TRUE)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
  else()
    set(IS_LINUX TRUE)
  endif()
endif()

if (IS_WIN32)
  set( wxWidgets_USE_STATIC ON)
  set(CODE_FILES win32/minimal.rc)
endif()

find_package(wxWidgets 2.8.10 REQUIRED COMPONENTS xml html adv qa richtext net core base)

include(${wxWidgets_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR})

find_package(LibFko REQUIRED)
find_package(CURL REQUIRED)

set(CODE_FILES
  ${CODE_FILES}
  configs.cpp
  fwknop_guiApp.cpp
  fwknop_guiMain.cpp
  )


add_executable(fwknop-gui WIN32 MACOSX_BUNDLE
  ${CODE_FILES}
  )

SET_TARGET_PROPERTIES(fwknop-gui PROPERTIES POSITION_INDEPENDENT_CODE ON)
add_definitions(-D_UNICODE)

target_link_libraries(fwknop-gui ${LIBFKO_LIBRARIES} ${CURL_LIBRARIES} ${wxWidgets_LIBRARIES})
if(IS_LINUX)
  install(FILES help.html DESTINATION share/fwknop-gui)
  install(TARGETS fwknop-gui DESTINATION bin)
  install(PROGRAMS fwknop-gui.desktop DESTINATION share/applications)
  install(FILES fwknop-gui.png DESTINATION share/pixmaps)
endif()
if(IS_WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()
if(IS_APPLE)
    INSTALL(CODE "
    set(BU_CHMOD_BUNDLE_ITEMS ON)
    include(BundleUtilities)
    fixup_bundle(${CMAKE_INSTALL_PREFIX}/fwknop-gui.app \"\" \"\")
")
endif()
