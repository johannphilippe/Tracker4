###############################################################################
#  Copyright (c) 2016-2020 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.9.6...3.15.0)
project(EmptyStarter LANGUAGES C CXX)

set(ELEMENTS_ROOT "/home/johann/Documents/Elements/elements")

if (NOT ELEMENTS_ROOT)
   message(FATAL_ERROR "ELEMENTS_ROOT is not set")
endif()

# Make sure ELEMENTS_ROOT is an absolute path to add to the CMake module path
get_filename_component(ELEMENTS_ROOT "${ELEMENTS_ROOT}" ABSOLUTE)
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${ELEMENTS_ROOT}/cmake")

# If we are building outside the project, you need to set ELEMENTS_ROOT:
if (NOT ELEMENTS_BUILD_EXAMPLES)
   include(ElementsConfigCommon)
   set(ELEMENTS_BUILD_EXAMPLES OFF)
   add_subdirectory(${ELEMENTS_ROOT} elements)
endif()

set(ELEMENTS_APP_PROJECT "Tracker")
set(ELEMENTS_APP_TITLE "Tracker")
set(ELEMENTS_APP_COPYRIGHT "Copyright (c) 2021 Johann Philippe")
set(ELEMENTS_APP_ID "com.johannphilippe.tracker")
set(ELEMENTS_APP_VERSION "1.0")

set(ELEMENTS_APP_SOURCES 

      # Models
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/curve_point.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/curve.h

      # Widgets
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/curve_editor.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/curve_editor.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_radio.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_content.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_content.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_base.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_labels.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/spinbox.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/spinbox.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_cell.h

      # Tracker Views
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/curve_editor_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/curve_editor_view.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tab_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tab_view.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tracker_track.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/sniper_track.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/track_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/track_view.cpp

      # Maths
      ${CMAKE_CURRENT_SOURCE_DIR}/src/maths/cubic_spline.h

      # Utilities
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/string_utilities.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/maths_utilities.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/paint_utilities.h


      # Global
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker.cpp

      ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
      )


  set(ELEMENTS_APP_INCLUDE_DIRECTORIES
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src/lib

      )

include(ElementsConfigApp)
