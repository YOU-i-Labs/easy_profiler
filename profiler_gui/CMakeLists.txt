#set(CMAKE_PREFIX_PATH f:/qt/5.5/5.6/msvc2013_64/lib/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(hunter_configuration_types)
hunter_configuration_types(Qt CONFIGURATION_TYPES Release)
hunter_add_package(Qt)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Widgets REQUIRED)

if (Qt5Widgets_FOUND)
    if (NOT("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") AND WIN32)
        set(APPLICATION_PLATFORM WIN32)
    endif ()
    add_executable(profiler_gui ${APPLICATION_PLATFORM}
        main.cpp
        arbitrary_value_inspector.h
        arbitrary_value_inspector.cpp
        arbitrary_value_tooltip.h
        arbitrary_value_tooltip.cpp
        blocks_graphics_view.h
        blocks_graphics_view.cpp
        blocks_tree_widget.h
        blocks_tree_widget.cpp
        bookmarks_editor.h
        bookmarks_editor.cpp
        common_functions.h
        common_functions.cpp
        common_types.h
        descriptors_tree_widget.h
        descriptors_tree_widget.cpp
        dialog.h
        dialog.cpp
        fps_widget.h
        fps_widget.cpp
        globals.h
        globals.cpp
        globals_qobjects.cpp
        graphics_block_item.h
        graphics_block_item.cpp
        graphics_image_item.h
        graphics_image_item.cpp
        graphics_ruler_item.h
        graphics_ruler_item.cpp
        graphics_scrollbar.h
        graphics_scrollbar.cpp
        graphics_slider_area.h
        graphics_slider_area.cpp
        main_window.h
        main_window.cpp
        round_progress_widget.h
        round_progress_widget.cpp
        timer.h
        timer.cpp
        thread_pool.h
        thread_pool.cpp
        thread_pool_task.h
        thread_pool_task.cpp
        tree_widget_item.h
        tree_widget_item.cpp
        tree_widget_loader.h
        tree_widget_loader.cpp
        window_header.h
        window_header.cpp
        resources.qrc
        resources.rc
    )

    if (APPLE)
        target_link_libraries(profiler_gui PUBLIC "-framework Security")
    endif()

    target_link_libraries(profiler_gui PUBLIC Qt5::Widgets Qt5::PrintSupport easy_profiler)
    if (WIN32)
        target_compile_definitions(profiler_gui PRIVATE -D_WIN32_WINNT=0x0600)
    endif ()
    if (MINGW)
        target_compile_definitions(profiler_gui PRIVATE -DSTRSAFE_NO_DEPRECATE)
    endif ()

    install(
        TARGETS
        profiler_gui
        CONFIGURATIONS
        Release
        RUNTIME
        DESTINATION
        bin
    )

    set_property(TARGET profiler_gui PROPERTY INSTALL_RPATH_USE_LINK_PATH TRUE)
else ()
    message(STATUS "INFO\n\n\tQt5 not found! Generating EasyProfiler projects without GUI.\n")
endif ()

