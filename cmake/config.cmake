hunter_config(ZLIB
        URL "https://github.com/YOU-i-Labs/zlib/archive/1.2.8-youi.zip"
        VERSION "1.2.8-youi"
        SHA1 "69ad25e0df68f7f450be26ff8dc8b3bb2ca8cf04"
        CMAKE_ARGS BUILD_SHARED_LIBS=OFF ZLIB_BUILD_SHARED=OFF ZLIB_SKIP_EXECUTABLES=ON)

        # Qt is a dependency of the easy_profiler GUI
if (LINUX)
        hunter_config(Qt
                CONFIGURATION_TYPES Release
                VERSION "5.5.1-cvpixelbuffer-2-p9"
        )
else()
        hunter_config(Qt
                VERSION "5.11.3"
                CONFIGURATION_TYPES Release
                CMAKE_ARGS FORCE_BUILD_AFTER_HUNTER_UPDATE=3
        )
endif()
