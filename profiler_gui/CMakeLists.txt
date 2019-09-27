#set(CMAKE_PREFIX_PATH f:/qt/5.5/5.6/msvc2013_64/lib/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(EASY_PROFILER_GUI_WITH_PREBUILT_QT "Use prebuilt Qt tarball to build the GUI" ON)

if (EASY_PROFILER_GUI_WITH_PREBUILT_QT AND OSX)
    
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/qt5
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    # Using Qt pre-builts because xcode10 and xcode11 have issues with building Qt not for the targetted IDEs SDK.
    # These prebuilts are from hunter_cache built on jenkins with XCode10 on OSX 10.14. This version of Qt works on 10.15 fine.
    # qt_5.11.3_osx10.14_prebuilt.tar.bz2   is   02209aee16a800a37a7705c38220e4c5326778fe.tar.bz2 from hunter
    # qt_5.11.3_osx10.14_cmakeextras.tar.bz2   is   946f04a12348edc49e0bbbf6fe865c7e2c5cc35e.tar.bz2 from hunter

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/qt_5.11.3_osx10.14_prebuilt.tar.bz2
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/qt_5.11.3_osx10.14_cmakeextras.tar.bz2
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/qt5
        RESULT_VARIABLE error_code
    )

    if(error_code)
        message(FATAL_ERROR "Failed to extarct qt prebuilt")
    endif()
      
    find_package(Qt5PrintSupport CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR}/qt5/lib/cmake/Qt5PrintSupport)
    find_package(Qt5Widgets CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR}/qt5/lib/cmake/Qt5Widgets)

else()
    include(hunter_configuration_types)
    hunter_configuration_types(Qt CONFIGURATION_TYPES Release)
    hunter_add_package(Qt)
    find_package(Qt5PrintSupport REQUIRED)
    find_package(Qt5Widgets REQUIRED)
endif()




if (LINUX)
    find_package(ZLIB REQUIRED)

    if (NOT ZLIB_FOUND)
        message(FATAL_ERROR "zlib library not found")
    endif()
endif()

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

    target_link_libraries(profiler_gui PUBLIC Qt5::Widgets Qt5::PrintSupport Qt5::Core Qt5::Gui easy_profiler)
    if (VS2017)
        target_link_libraries(profiler_gui PUBLIC "${QT_ROOT}/lib/Qt5WindowsUIAutomationSupport.lib")
    endif()
    if (WIN32)
        target_compile_definitions(profiler_gui PRIVATE -D_WIN32_WINNT=0x0600)
    endif ()
    if (MINGW)
        target_compile_definitions(profiler_gui PRIVATE -DSTRSAFE_NO_DEPRECATE)
    endif ()
    if (LINUX)
        target_link_libraries(profiler_gui PUBLIC ZLIB::zlib)
    endif()

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

