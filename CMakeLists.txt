###############################################################################
#  Copyright (c) 2016-2020 Joel de Guzman
#
#  Distributed under the MIT License (https://opensource.org/licenses/MIT)
###############################################################################
cmake_minimum_required(VERSION 3.9.6...3.15.0)
project(DTracker LANGUAGES C CXX)

if (NOT ELEMENTS_ROOT)
   message(WARNING "ELEMENTS_ROOT is not set")
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

set(ELEMENTS_APP_PROJECT "DTracker")
set(ELEMENTS_APP_TITLE "DTracker")
set(ELEMENTS_APP_COPYRIGHT "Copyright (c) 2021 Johann Philippe")
set(ELEMENTS_APP_ID "com.johannphilippe.tracker")
set(ELEMENTS_APP_VERSION "1.0")

add_compile_definitions(TRACKER_APP_NAME="DTracker")

set(ELEMENTS_APP_SOURCES 
      # Audio
      #${CMAKE_CURRENT_SOURCE_DIR}/src/audio/csound_singleton.h
      #${CMAKE_CURRENT_SOURCE_DIR}/src/audio/csound_singleton.cpp

      # Animations
      ${CMAKE_CURRENT_SOURCE_DIR}/src/animations/generic_animation.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/animations/generic_animation.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/animations/fps_model.h

      # Controllers
      ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/cell_selection_controller.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/cell_selection_controller.cpp

      #Dialogs
      ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/options_dialog.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/options_dialog.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/elements_dialog_base.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs/elements_dialog_base.cpp

      # Models
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/curve_point.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/curve.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/track_event.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/models/sniper_track_event.h

      # Widgets
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/curve_editor.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/curve_editor.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_radio.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_radio.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_content.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_content.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_track_content.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_track_content.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_base.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_labels.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/spinbox.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/spinbox.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_cell.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_cell.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/dynamic_size_cell_composer.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_track_box.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_track_box.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_event_editor.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/sniper_event_editor.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/custom_container.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/expandable.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/expandable.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_line.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/track_line.cpp

      # Tracker Widgets
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/curve_editor_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/curve_editor_view.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tab_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tab_view.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tracker_track.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tracker_track.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/sniper_track.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/track_view.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/track_view.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tracker_menus.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tempo_track.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tempo_track.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tempo_track_line.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/widgets/tracker/tempo_track_line.cpp

      # Maths
      ${CMAKE_CURRENT_SOURCE_DIR}/src/maths/cubic_spline.h

      # Utilities
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/string_utilities.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/maths_utilities.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/paint_utilities.h

      # Tracker
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker_theme.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker_theme.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker_globals.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/jtracker_globals.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/filesystem.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/config_data.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/config_data.cpp
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/tracker_actions.h
      ${CMAKE_CURRENT_SOURCE_DIR}/src/tracker/tracker_actions.cpp

      ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
      )


  set(ELEMENTS_APP_INCLUDE_DIRECTORIES
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src/lib
      )

include(ElementsConfigApp)
